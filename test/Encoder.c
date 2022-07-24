#include "Encoder.h"

#define CLK PB4
#define DT PC7
#define BUTTON PA5

#define CLK_DDR DDRB
#define CLK_PORT PORTB
#define CLK_PIN 4
#define DT_DDR DDRC
#define DT_PORT PORTC
#define DT_PIN 7
#define BUTTON_DDR DDRA
#define BUTTON_PORT PORTA
#define BUTTON_PIN 5

#define clk						0b00010000
#define is_clk_down				!(PINB&clk)
#define dt						0b10000000
#define is_dt_down				!(PINC&dt) 
#define sw						0b00100000
#define is_button_encoder_pres  !(PINA&sw)

#define NO_ACTION			0
#define LONG_PRESS_BUTTON	1
#define PRESS_BUTTON		2
#define ROTATE_LEFT			3
#define ROTATE_RIGHT		4

//*************инициализация портов******************
void portInit(){
	//CLK_DDR | = (1<<CLK_PIN);
	//CLK_PORT & = ~(1<<CLK_PIN);
	//DT_DDR | = (1<<DT_PIN);
	//DT_PORT & = ~(1<<DT_PIN);
	//BUTTON_DDR | = (1<<BUTTON_PIN);
	//BUTTON_PORT & = ~(1<<BUTTON_PIN);
}

//****************состояние пинов*********************
char statePins(){
	if(!is_clk_down && !is_dt_down) //состояние покоя 11
		return 0x03;
	if(is_clk_down && is_dt_down) //00
		return 0;
	if(is_clk_down && !is_dt_down) //01
		return 0x01;
	if(!is_clk_down && is_dt_down) //10
		return 0x02;
}
 
 //***********отключаем таймер энкодера****************
 void timerOff(){
	 TCCR0 &= ~((1<<CS12)|(1<<CS10));
 }
 
 //***************включаем таймер энкодера*****************
 void timerOn(){
	 TCCR0 |= (1<<CS12)|(1<<CS10);
 }
 
//******************инициализация таймера***********************  
 void timer_init(void)
 {
 TCCR0 |= (1<<WGM01); // устанавливаем режим СТС (сброс по совпадению)
 TIMSK |= (1<<OCIE0); //разрешение прерывания сравнения совпадения выхода таймера/счетчика 0
 OCR0 = 0b00010000; //записываем в регистр число 16 для сравнения.(2ms) 0b00010000
 TCCR0 |= (1<<CS12)|(1<<CS10);//установим делитель на 1024, выставим бит CS12 и CS10 в 1. на этот делитель мы поделим 8000000
 }  
   
unsigned int countTic = 0; //счетчик тиков 

const char lengthQueue = 10; //длина массива состояний
char queueStateEncoder[10];// = {0,0,0,0,0,0,0,0,0,0}; // массив состояний
char flagPressButton = 0; //флаг нажатой кнопки
	
//*********ложим данные в очередь ***************
void putInQueue(char stateEnc){
	if(queueStateEncoder[lengthQueue-1] ) return; //если переполнение очереди, выходим
	for(char i = 0; i < lengthQueue; ++i){
		if(queueStateEncoder[i] == 0){ //нашли свободную ячейку
			queueStateEncoder[i] = stateEnc; //положили в нее данные
			return;
		}
	}
}

//********извлекаем данные с очереди*************
char takeFromTheQueue(){
	if(queueStateEncoder[0] == 0) return NO_ACTION;
	char result = queueStateEncoder[0]; //забираем первого с очереди 
	for(char i = 0; i < lengthQueue; ++i){
		if(queueStateEncoder[i] == 0) continue;
		if(queueStateEncoder[i] != lengthQueue - 1){ //если это не последний элемент
			if(queueStateEncoder[i+1] ) //если следующий элемент не 0
				queueStateEncoder[i] = queueStateEncoder[i+1]; //следующий элемент стаёт текущим
			else	//следующий элемент =0
				queueStateEncoder[i] = 0;
		}else queueStateEncoder[i] = 0; //если последний элемент
	}
	return result;
}
 
 //***********************обработчик прерываний от таймера****************
ISR (TIMER0_COMP_vect)
{ 
	if(is_button_encoder_pres){  //когда нажата кнопка
		flagPressButton = 1; //флаг в 1
		if(countTic < 110) //что бы небыло переполнения счетчика при заклинивании кнопки
			++countTic;  //счетчик включен
	}
	if(flagPressButton && !is_button_encoder_pres){ //кнопка отпущена и флаг стоит
		//если нажатие более 200мс то долгое нажатие
		(countTic > 100)? putInQueue(LONG_PRESS_BUTTON):putInQueue(PRESS_BUTTON);
		countTic = 0;
		flagPressButton = 0;
	}
	
	static unsigned char new_state=0;
	static unsigned char old_state=0;
	
	new_state = statePins();
	
	switch(old_state | new_state) //если раскоментировать будут по +4 за тик
	{
		case 0x07: //case 0x0e: case 0x08: case 0x01:
		putInQueue(ROTATE_LEFT);
		break;
		case 0x0b: //case 0x04: case 0x0d: case 0x02:
		putInQueue(ROTATE_RIGHT);
		break;
	}
	old_state = new_state << 2;
}

//****************возвращаем статус енкодера ****************
 char getStateEncoder(){
	return takeFromTheQueue();
 }

//**************состояние кнопки энкодера**********************-
char getPressButtonEncoder(){
if(is_button_encoder_pres)
	return 1;
else 
	return 0;	
}

//***********инициализация работы с энкодером***************
void initEncoder(){
	timer_init();
	timerOff(); //таймер выключаем
	//portInit();
}

