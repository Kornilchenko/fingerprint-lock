#include "main.h"


//**********************Инициализация портов*****************
void port_ini(void)
{
	/*
	0,1,2 - АЦП.  in 0
	3 - реле заряда. out 0
	4 - ик датчик. in 0
	5 - BUTTON энкодера in 1 / резерв.K3 out 0
	6 - концевой двери. in 1
	7 - концевой замка. in 1
	*/
	DDRA =  0b00001000;
	PORTA = 0b11100000;
	/*
	0,1,2 - балансир. out 0
	3 - кнопка CHARGE. in 1
	4 - CLK энкодера in 1 /  резерв K1. out 0
	5,6,7 - для програмирования. in 0
	*/
	DDRB =  0b00000111;
	PORTB = 0b00011000;
	/*
	0,1 - I2C. out 0
	2,3 - реле замка двери. out 0
	4,5 - светодиоды кнопки out 0
	6 - кнопка внутри. in 1
	7 - DT энкодера in 1 / резерв.K1 out 0
	*/
	DDRC =  0b00111111;
	PORTC = 0b11000000;
	/*
	0 - подключение USART RXD. in 1
	1 - подключение USART TXD. out 1
	2,3,4 - не задействованы. out 0
	5 - RST SIM800. out 0
	6 - RING SIM800. out 0
	7 - DTR SIM800. out 0
	*/
	DDRD =  0b11111110;
	PORTD = 0b00000011;
}

//-------------------------------------------------------
int main(void)
{
	_delay_ms(500);
	port_ini(); //Инициализируем порты
	I2C_Init();//инициализируем TWI
	display_128x64_Init(); // инициализация OLED
	USART_Init (16);//57600
	initFingerScaner(); //инициализация сканера
	displayClear();
	timer_ini(); //Инициализируем таймер
	ADC_Init(); //Инициализируем АЦП
	initWorkBattery(); //инициализация работы с батареями
	initEncoder(); //инициализация работы с энкодером
	initParametrDoor(); //чтение уставок для работы двери, инициализация
	sei();
	_delay_ms(700);
/*	---------------------------*/
	unsigned char data[28];
	char resultSearchFinger = 1;
	char result[10];
	char count = 0; //
	
	displayShowStr(3,3,"запускаем прогу");
	_delay_ms(700);
	displayClear();
	
	while (1)
	{
//*******************отпечатки******************
		if (is_closed_door){
			displayShowStr(0,2,"Отпечаток не опознан");
			if(is_open_lock){
				resultSearchFinger = searchFingerInSensor();
				if(!resultSearchFinger)
				closeLock();
			}
			resultSearchFinger = searchFingerR300();
			if(is_closed_lock){
				resultSearchFinger = searchFingerR300();
				if (resultSearchFinger){
					displayShowStr(0,2,"Отпечаток опознан   ");
					openLock();
				}else
				displayShowStr(0,2,"Отпечаток не опознан");
			}
		}else
		displayShowStr(0,2,"Сканер отключен     ");
//****************двери************************* 
		 workCloseOpenDoorGeneral();
		 
		if(count%10 == 1){ //каждый 10 цикл измеряет и отображает напряжение
			//**************работа с акумами****************
			workWithBatteries();
	//отображаем напряжения
			sprintf(result,"%.2fB", getVoltage(1));
			displayShowStr(0,5,"Б1 =");
			displayShowStr(4,5, result);
	 
			sprintf(result,"%.2fB", getVoltage(2));
			displayShowStr(0,6,"Б2 =");
			displayShowStr(4,6, result);
	 
			sprintf(result,"%.2fB", getVoltage(3));
			displayShowStr(0,7,"Б3 =");
			displayShowStr(4,7, result);
	 
			sprintf(result,"%.2fB",getVoltage(4));
			displayShowStr(0,4,"U =");
			displayShowStr(4,4, result);
	 
			if(getChargeMode()){
				displayShowStr(14,4,"заряд ");
				if(getBypassMode(1))
					displayShowStr(12,5,"байпас");
				else
					displayShowStr(12,5,"      ");
				if(getBypassMode(2))
					displayShowStr(12,6,"байпас");
				else
					displayShowStr(12,6,"      ");
				if(getBypassMode(3))
					displayShowStr(12,7,"байпас");
				else
					displayShowStr(12,7,"      ");	 
			}else{
				displayShowStr(14,4,"разряд");
				displayShowStr(12,5,"      ");
				displayShowStr(12,6,"      ");
				displayShowStr(12,7,"      ");
			}
		}

//********************  menu ******************************	 
		 if(getPressButtonEncoder()){ //кнопка энкодера нажата
			while(getPressButtonEncoder());
			showMenuGeneral();
		 }  
		 ++count;
		 if(count >=200)     
			 count = 0;                                                                                              
	 }
 }