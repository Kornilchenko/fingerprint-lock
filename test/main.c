#include "main.h"


//**********************������������� ������*****************
void port_ini(void)
{
	/*
	0,1,2 - ���.  in 0
	3 - ���� ������. out 0
	4 - �� ������. in 0
	5 - BUTTON �������� in 1 / ������.K3 out 0
	6 - �������� �����. in 1
	7 - �������� �����. in 1
	*/
	DDRA =  0b00001000;
	PORTA = 0b11100000;
	/*
	0,1,2 - ��������. out 0
	3 - ������ CHARGE. in 1
	4 - CLK �������� in 1 /  ������ K1. out 0
	5,6,7 - ��� ���������������. in 0
	*/
	DDRB =  0b00000111;
	PORTB = 0b00011000;
	/*
	0,1 - I2C. out 0
	2,3 - ���� ����� �����. out 0
	4,5 - ���������� ������ out 0
	6 - ������ ������. in 1
	7 - DT �������� in 1 / ������.K1 out 0
	*/
	DDRC =  0b00111111;
	PORTC = 0b11000000;
	/*
	0 - ����������� USART RXD. in 1
	1 - ����������� USART TXD. out 1
	2,3,4 - �� �������������. out 0
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
	port_ini(); //�������������� �����
	I2C_Init();//�������������� TWI
	display_128x64_Init(); // ������������� OLED
	USART_Init (16);//57600
	initFingerScaner(); //������������� �������
	displayClear();
	timer_ini(); //�������������� ������
	ADC_Init(); //�������������� ���
	initWorkBattery(); //������������� ������ � ���������
	initEncoder(); //������������� ������ � ���������
	initParametrDoor(); //������ ������� ��� ������ �����, �������������
	sei();
	_delay_ms(700);
/*	---------------------------*/
	unsigned char data[28];
	char resultSearchFinger = 1;
	char result[10];
	char count = 0; //
	
	displayShowStr(3,3,"��������� �����");
	_delay_ms(700);
	displayClear();
	
	while (1)
	{
//*******************���������******************
		if (is_closed_door){
			displayShowStr(0,2,"��������� �� �������");
			if(is_open_lock){
				resultSearchFinger = searchFingerInSensor();
				if(!resultSearchFinger)
				closeLock();
			}
			resultSearchFinger = searchFingerR300();
			if(is_closed_lock){
				resultSearchFinger = searchFingerR300();
				if (resultSearchFinger){
					displayShowStr(0,2,"��������� �������   ");
					openLock();
				}else
				displayShowStr(0,2,"��������� �� �������");
			}
		}else
		displayShowStr(0,2,"������ ��������     ");
//****************�����************************* 
		 workCloseOpenDoorGeneral();
		 
		if(count%10 == 1){ //������ 10 ���� �������� � ���������� ����������
			//**************������ � �������****************
			workWithBatteries();
	//���������� ����������
			sprintf(result,"%.2fB", getVoltage(1));
			displayShowStr(0,5,"�1 =");
			displayShowStr(4,5, result);
	 
			sprintf(result,"%.2fB", getVoltage(2));
			displayShowStr(0,6,"�2 =");
			displayShowStr(4,6, result);
	 
			sprintf(result,"%.2fB", getVoltage(3));
			displayShowStr(0,7,"�3 =");
			displayShowStr(4,7, result);
	 
			sprintf(result,"%.2fB",getVoltage(4));
			displayShowStr(0,4,"U =");
			displayShowStr(4,4, result);
	 
			if(getChargeMode()){
				displayShowStr(14,4,"����� ");
				if(getBypassMode(1))
					displayShowStr(12,5,"������");
				else
					displayShowStr(12,5,"      ");
				if(getBypassMode(2))
					displayShowStr(12,6,"������");
				else
					displayShowStr(12,6,"      ");
				if(getBypassMode(3))
					displayShowStr(12,7,"������");
				else
					displayShowStr(12,7,"      ");	 
			}else{
				displayShowStr(14,4,"������");
				displayShowStr(12,5,"      ");
				displayShowStr(12,6,"      ");
				displayShowStr(12,7,"      ");
			}
		}

//********************  menu ******************************	 
		 if(getPressButtonEncoder()){ //������ �������� ������
			while(getPressButtonEncoder());
			showMenuGeneral();
		 }  
		 ++count;
		 if(count >=200)     
			 count = 0;                                                                                              
	 }
 }