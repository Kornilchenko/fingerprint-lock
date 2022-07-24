#include "work_with_batteries.h"

unsigned int bat1; //���������� �������� ���
unsigned int bat2;
unsigned int bat3;

float coefficientBat1; 
float coefficientBat2;
float coefficientBat3;

float minimumVoltage; //����������� ���������� ������� �� �������
float maximumVoltage; //������������ ���������� ������ �� �������

char charge_mode = 0;//���������� ������� �������

char bypassMode1 = 0;//���������� ������� ��������� ������� B1
char bypassMode2 = 0;//���������� ������� ��������� ������� B2
char bypassMode3 = 0;//���������� ������� ��������� ������� B3

unsigned int minimumValueForDischargingBat1;
unsigned int minimumValueForDischargingBat2;
unsigned int minimumValueForDischargingBat3;
unsigned int maximumValueForChargingBat1;
unsigned int maximumValueForChargingBat2;
unsigned int maximumValueForChargingBat3;
unsigned int maximumValueForCharging;

//---------���� ������� ------------------
#define rele_charge PA3
#define rele_charge_on  PORTA |=  (1<<rele_charge)
#define rele_charge_off PORTA &= ~(1<<rele_charge)

//-------------��������� -------------------------
#define bypass_BAT1 PB1
#define bypass_BAT1_on PORTB |=  (1<<bypass_BAT1)
#define bypass_BAT1_off PORTB &= ~(1<<bypass_BAT1)

#define bypass_BAT2 PB0
#define bypass_BAT2_on PORTB |=  (1<<bypass_BAT2)
#define bypass_BAT2_off PORTB &= ~(1<<bypass_BAT2)

#define bypass_BAT3 PB2
#define bypass_BAT3_on PORTB |=  (1<<bypass_BAT3)
#define bypass_BAT3_off PORTB &= ~(1<<bypass_BAT3)

//-------------������ �������------------------
#define button_charge		0b00001000
#define isButtonChargePresed !(PINB&button_charge)

//----------------��������� �������� ���������� � ���------------------------
void readBatteryVoltage(){
	//�������� ���� ADC0 ADMUX = 11000000;
	ADMUX &=~(1<<MUX1);
	ADMUX &=~(1<<MUX0);
	_delay_ms(10);
	bat1 = (unsigned int)get_ADC_value();
	//�������� ���� ADC2 ADMUX = 11000010;
	ADMUX &=~(1<<MUX0);
	ADMUX |=(1<<MUX1);
	_delay_ms(10);
	bat2 = (unsigned int)get_ADC_value();
	//�������� ���� ADC1 ADMUX = 11000001;
	ADMUX &=~(1<<MUX1);
	ADMUX |=(1<<MUX0);
	_delay_ms(10);
	bat3 = (unsigned int)get_ADC_value();
}

//****************��������� ������� �� ������**********************
void changeBattery(){
	if (bat1<minimumValueForDischargingBat1) //��������� ���������� �� ������� �1 ������ 2.75�
	{
		charge_mode=1; //������ ������ �� 1
		rele_charge_on;//�������� ���� ������� �1
	}
	if (bat2-bat1<minimumValueForDischargingBat2) //��������� ���������� �� ������� �2
	{
		charge_mode=1; //������ ������ �� 1
		rele_charge_on;//�������� ���� ������� �1
	}
	if (bat3-bat2<minimumValueForDischargingBat3) //��������� ���������� �� ������� �3
	{
		charge_mode=1; //������ ������ �� 1
		rele_charge_on;//�������� ���� ������� �1
	}
}

//**********************��������� �������� �������*****************
void workBypass(){
	if (charge_mode == 1){ //���� ������� ����� �������
		if (bat1 > maximumValueForChargingBat1){//��������� ���������� �� �������� �1
			bypassMode1 = 1; //���������� ������ 1 � �������� ������ �������
			bypass_BAT1_on;
		}else{
			bypassMode1 = 0; //���������� ������ 0 � ��������� ������ �������
			bypass_BAT1_off;
		}
		if (bat2 - bat1 > maximumValueForChargingBat2){ //��������� ���������� �� �������� �2
			bypassMode2 = 1; //���������� ������ 1 � �������� ������ �������
			bypass_BAT2_on;
		}else{
			bypassMode2 = 0; //���������� ������ 0 � ��������� ������ �������
			bypass_BAT2_off;
		}
		if (bat3 - bat2 > maximumValueForChargingBat3){ //��������� ���������� �� �������� �3
			bypassMode3 = 1; //���������� ������ 1 � �������� ������ �������
			bypass_BAT3_on;
		}else{
			bypassMode3 = 0; //���������� ������ 0 � ��������� ������ �������
			bypass_BAT3_off;
		}
	}	
}

//****************�������-��������� ������� ������� �� ������� �� ������********
void pushButtonCharging(){ 
	if (charge_mode == 0 && isButtonChargePresed)
	{
			while (isButtonChargePresed); //���� ���������� ������
			rele_charge_on;
			charge_mode = 1;
	}
	else if (charge_mode == 1 && isButtonChargePresed)
	{
		while (isButtonChargePresed); //���� ���������� ������
		rele_charge_off;
		charge_mode = 0; 
	}
}

//****************�������� ������ �������****************************
void endOfChargeControl(){
//��������� ���������� ���� �������
	if (charge_mode && bat3 > maximumValueForCharging){
		rele_charge_off;
		charge_mode = 0; //������ ������ �� 0
		bypass_BAT1_off; //��������� ������� �������
		bypass_BAT2_off;
		bypass_BAT3_off;
		bypassMode1 = 0;
		bypassMode2 = 0;
		bypassMode3 = 0;
	}	
}	

//*******������ ����������� �������������� �������� ���-��������� � eeprom*******
void recordOfCoefficientsEEPROM(float voltage, char numBatery){
	readBatteryVoltage();
	unsigned int address = 0;
	unsigned int batteryADC = 0;
	unsigned int word = 0;
	float coefficient = 0;
	switch(numBatery)
	{
		case 1: address = 800;
				batteryADC = bat1;
				break;
		case 2: address = 804; 
				batteryADC = bat2 - bat1;
				break;
		case 3: address = 808; 
				batteryADC = bat3 - bat2;
				break;
	}
	coefficient = (float)batteryADC / voltage;
	word = (int)coefficient; //�������� � ���������� ����� �����
	EEPROM_write_word(address, word);
	coefficient -= word; 
	word = coefficient * 10000; //�������� ������� ����� � ���������� �
	EEPROM_write_word(address + 2, word);
}

//**************������ ������������ �� EEPROM **********************
void readingCoefficientsEEPROM(){
	coefficientBat1 = EEPROM_read_word(800) + (float)EEPROM_read_word(802)/10000.0;
	coefficientBat2 = EEPROM_read_word(804) + (float)(EEPROM_read_word(806)/10000.0);
	coefficientBat3 = EEPROM_read_word(808) + (float)(EEPROM_read_word(810)/10000.0);
	minimumVoltage =  EEPROM_read(812) + (float)(EEPROM_read(813)/100.0);
	maximumVoltage = EEPROM_read(814) + (float)(EEPROM_read(815)/100.0); 
}

//****************������������� ������ � ���������*******************
void initWorkBattery(){
	readingCoefficientsEEPROM();
	minimumValueForDischargingBat1 = coefficientBat1 * minimumVoltage;
	minimumValueForDischargingBat2 = coefficientBat2 * minimumVoltage;
	minimumValueForDischargingBat3 = coefficientBat3 * minimumVoltage;
	maximumValueForChargingBat1 = coefficientBat1 * maximumVoltage;
	maximumValueForChargingBat2 = coefficientBat2 * maximumVoltage;
	maximumValueForChargingBat3 = coefficientBat3 * maximumVoltage;
	//������������ ���������� ��� 99% �� ����� ���������� �� 3 ��������
	maximumValueForCharging = ((maximumValueForChargingBat1 +
								maximumValueForChargingBat2 +
								maximumValueForChargingBat3) * 0.99);
}

//**********************������ � ��������������***********************
void workWithBatteries(){
	readBatteryVoltage(); //������ ����������
	pushButtonCharging(); //������������ ������
	changeBattery(); //��������� ������� ��������� �������
	workBypass(); //������ �������
	endOfChargeControl(); //�������� ��������� �������
}

//*****************************getVoltage*****************************
float getVoltage(char numBatery){
	unsigned int batteryADC = 0;
	float coefficient = 0 ;
	switch(numBatery)
	{
		case 1: coefficient = coefficientBat1;
				batteryADC = bat1;
				break;
		case 2: coefficient = coefficientBat2;
				batteryADC = bat2 - bat1;
				break;
		case 3: coefficient = coefficientBat3;
				batteryADC = bat3 - bat2;
				break;
		case 4: return (float)bat1/coefficientBat1+ 
				(float)(bat2-bat1) / coefficientBat2 +
				(float)(bat3-bat2) / coefficientBat3;
				break;
	}
	return batteryADC / coefficient;
}

//**************** ��������� ������ ������� **********************
char getChargeMode(){return charge_mode;}
	
//**************** ��������� �������� ****************************
char getBypassMode(char numBatery){
	char bypass = 0;
	switch(numBatery)
	{
		case 1: bypass = bypassMode1;
				break;
		case 2: bypass = bypassMode2;
				break;
		case 3: bypass = bypassMode3;
				break;
	}
	return bypass;
}

//***************��������� ����������� **************************
void setCoefficient(float voltage, char numBatery){
	if (numBatery == 1){
		recordOfCoefficientsEEPROM(voltage, 1);
		coefficientBat1 = bat1 / voltage;
	}
	else if(numBatery == 2){
		recordOfCoefficientsEEPROM(voltage, 2);
		coefficientBat2 = (bat2-bat1) / voltage;
	}
	else if (numBatery == 3){
		recordOfCoefficientsEEPROM(voltage, 3);
		coefficientBat3 = (bat3-bat2) / voltage;
	}
}

//***************��������� ������������ ���������� ������� ****************
void setMinimumVoltage(float voltage){
	unsigned char data = (int)voltage; //�������� � ���������� ����� �����
	EEPROM_write(812, data);
	voltage -= data;
	data = voltage * 100; //�������� ������� ����� � ���������� �
	EEPROM_write(813, data);
	initWorkBattery();
}

//***************��������� ������������� ���������� ������ *********
void setMaximumVoltage(float voltage){
	unsigned char data = (int)voltage; //�������� � ���������� ����� �����
	EEPROM_write(814, data);
	voltage -= data;
	data = voltage * 100; //�������� ������� ����� � ���������� �
	EEPROM_write(815, data);
	initWorkBattery();
}