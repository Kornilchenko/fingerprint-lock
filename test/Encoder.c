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

//*************������������� ������******************
void portInit(){
	//CLK_DDR | = (1<<CLK_PIN);
	//CLK_PORT & = ~(1<<CLK_PIN);
	//DT_DDR | = (1<<DT_PIN);
	//DT_PORT & = ~(1<<DT_PIN);
	//BUTTON_DDR | = (1<<BUTTON_PIN);
	//BUTTON_PORT & = ~(1<<BUTTON_PIN);
}

//****************��������� �����*********************
char statePins(){
	if(!is_clk_down && !is_dt_down) //��������� ����� 11
		return 0x03;
	if(is_clk_down && is_dt_down) //00
		return 0;
	if(is_clk_down && !is_dt_down) //01
		return 0x01;
	if(!is_clk_down && is_dt_down) //10
		return 0x02;
}
 
 //***********��������� ������ ��������****************
 void timerOff(){
	 TCCR0 &= ~((1<<CS12)|(1<<CS10));
 }
 
 //***************�������� ������ ��������*****************
 void timerOn(){
	 TCCR0 |= (1<<CS12)|(1<<CS10);
 }
 
//******************������������� �������***********************  
 void timer_init(void)
 {
 TCCR0 |= (1<<WGM01); // ������������� ����� ��� (����� �� ����������)
 TIMSK |= (1<<OCIE0); //���������� ���������� ��������� ���������� ������ �������/�������� 0
 OCR0 = 0b00010000; //���������� � ������� ����� 16 ��� ���������.(2ms) 0b00010000
 TCCR0 |= (1<<CS12)|(1<<CS10);//��������� �������� �� 1024, �������� ��� CS12 � CS10 � 1. �� ���� �������� �� ������� 8000000
 }  
   
unsigned int countTic = 0; //������� ����� 

const char lengthQueue = 10; //����� ������� ���������
char queueStateEncoder[10];// = {0,0,0,0,0,0,0,0,0,0}; // ������ ���������
char flagPressButton = 0; //���� ������� ������
	
//*********����� ������ � ������� ***************
void putInQueue(char stateEnc){
	if(queueStateEncoder[lengthQueue-1] ) return; //���� ������������ �������, �������
	for(char i = 0; i < lengthQueue; ++i){
		if(queueStateEncoder[i] == 0){ //����� ��������� ������
			queueStateEncoder[i] = stateEnc; //�������� � ��� ������
			return;
		}
	}
}

//********��������� ������ � �������*************
char takeFromTheQueue(){
	if(queueStateEncoder[0] == 0) return NO_ACTION;
	char result = queueStateEncoder[0]; //�������� ������� � ������� 
	for(char i = 0; i < lengthQueue; ++i){
		if(queueStateEncoder[i] == 0) continue;
		if(queueStateEncoder[i] != lengthQueue - 1){ //���� ��� �� ��������� �������
			if(queueStateEncoder[i+1] ) //���� ��������� ������� �� 0
				queueStateEncoder[i] = queueStateEncoder[i+1]; //��������� ������� ���� �������
			else	//��������� ������� =0
				queueStateEncoder[i] = 0;
		}else queueStateEncoder[i] = 0; //���� ��������� �������
	}
	return result;
}
 
 //***********************���������� ���������� �� �������****************
ISR (TIMER0_COMP_vect)
{ 
	if(is_button_encoder_pres){  //����� ������ ������
		flagPressButton = 1; //���� � 1
		if(countTic < 110) //��� �� ������ ������������ �������� ��� ������������ ������
			++countTic;  //������� �������
	}
	if(flagPressButton && !is_button_encoder_pres){ //������ �������� � ���� �����
		//���� ������� ����� 200�� �� ������ �������
		(countTic > 100)? putInQueue(LONG_PRESS_BUTTON):putInQueue(PRESS_BUTTON);
		countTic = 0;
		flagPressButton = 0;
	}
	
	static unsigned char new_state=0;
	static unsigned char old_state=0;
	
	new_state = statePins();
	
	switch(old_state | new_state) //���� ���������������� ����� �� +4 �� ���
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

//****************���������� ������ �������� ****************
 char getStateEncoder(){
	return takeFromTheQueue();
 }

//**************��������� ������ ��������**********************-
char getPressButtonEncoder(){
if(is_button_encoder_pres)
	return 1;
else 
	return 0;	
}

//***********������������� ������ � ���������***************
void initEncoder(){
	timer_init();
	timerOff(); //������ ���������
	//portInit();
}

