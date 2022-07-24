#include "ssd_1306E.h"

//--------������� ��� ������� 
#define SSD1306_adress      0b01111000 //����� ���������� �� ����
#define COM					0b10000000  //��������� ������ ����� �������
#define DAT					0b11000000  //��������� ������ ����� ���� ������ 
#define DATS				0b01000000  //��� ����������� ����� ����� �������


//	������� �������� ������� �������
//-------------------------------------------------------------------------
void ssd1306_send_com(char com)
{
	I2C_StartCondition();
	I2C_SendByte(SSD1306_adress);
	I2C_SendByte(COM);
	I2C_SendByte(com);
	I2C_StopCondition();
}

//	������� �������� ������ �������
//-------------------------------------------------------------------------
void ssd1306_send_data(char data)
{
	I2C_StartCondition();
	I2C_SendByte(SSD1306_adress);
	I2C_SendByte(DAT);
	I2C_SendByte(data);
	I2C_StopCondition();
}

//	������� ��������������� �������. 
//-------------------------------------------------------------------------
void display_128x64_Init(void)
{
	_delay_ms(500);
	I2C_StartCondition();
	I2C_SendByte(SSD1306_adress);
	I2C_SendByte(COM);
	I2C_SendByte(0xAE);   //*��������� �������
	
	I2C_SendByte(COM);
	I2C_SendByte(0xA8); //*���������� multiplex ratio
	I2C_SendByte(COM);
	I2C_SendByte(0x3F);//0x3F*--1/64 duty (�������� �� ���������)
	
	I2C_SendByte(COM);
	I2C_SendByte(0xD3); //���������� ������������ �����(�������� ������� (offset))
	I2C_SendByte(COM);
	I2C_SendByte(0x00); //������������� ������������ ����� �� 00 (��� ��������)
	
	I2C_SendByte(COM);
	I2C_SendByte(0x40); //���������� ��������� ������ �������(� ������ RAM)
	
	I2C_SendByte(COM);
	I2C_SendByte(0xA1); //����� ��������� �� �������� (�� X) A1 - ���������� ����� (�����/�������) A0 - �������� (������/������)
	I2C_SendByte(COM);
	I2C_SendByte(0xC8); //������� ������ ��������� C8-������ ����, �0-����� �����, ����� ������������ ��� ������� //c0
	// ��� ��������� ������� ���������
	// �0 - �����/���� (������ ������ ����� ����)
	// �8 - ������/���� (������ ������� ����� ����)
	
	I2C_SendByte(COM);
	I2C_SendByte(0xDA); //���������� ������������ ������������ COM-���������
	I2C_SendByte(COM);
	I2C_SendByte(0x12); //(�����), �������������� ������������ ��������� COM
	
	I2C_SendByte(COM);
	I2C_SendByte(0x81);  //���������� �������� �������������
	I2C_SendByte(COM);
	I2C_SendByte(0x70); //������������� �������� FF (0-255)
	
	I2C_SendByte(COM);
	I2C_SendByte(0xA4); //���� ������� ������� ����������� ����������� ����������� ��� (�����)
	
	I2C_SendByte(COM);
	I2C_SendByte(0xA6); //���������� ������� (�����) ��� ��������
	
	I2C_SendByte(COM);
	I2C_SendByte(0xD9);   //��������� ��� DC/DC ���������������
	I2C_SendByte(COM);
	I2C_SendByte(0xF1);   //0x22-VCC �������� ����� / 0xF1 ��� �����������
	
	I2C_SendByte(COM);
	I2C_SendByte(0xD5); //���������� ����������� ������� ������� ������� / ������� �����������
	I2C_SendByte(COM);
	I2C_SendByte(0x80); //���������� ������� �����������, FOSC.(�����), ���������� ����������� ������� (D) ������������ ����� (DCLK): RESET
	
	I2C_SendByte(COM);
	I2C_SendByte(0x8D);// ���������� ���������� ����������������
	I2C_SendByte(COM);
	I2C_SendByte(0x14);//0x10 - ��������� (VCC �������� �����) 0x14 - ��������� ���������� DC/DC
	
	I2C_SendByte(COM);
	I2C_SendByte(0xDB);   // ��������� VcomH
	I2C_SendByte(COM);
	I2C_SendByte(0x40);   // ������ �� ������� ������� 0x30 (0x00-0x10-0x20-0x30-0x40-0x50-0x60-0x70)
	
	I2C_SendByte(COM);
	I2C_SendByte(0x20);   // ������ �������������� ���������
	I2C_SendByte(0x00);      // 0-�� ����������� � ��������� �� ����� �������� (������)
	//1 - �� ��������� � ��������� �� ����� ������
	//2 - ������ �� ��������� �������� ��� ��������
	
	//    OLEDwrite_comm(0x22);        // ������ ������� �� Y
	//   OLEDwrite_comm(0);           // �� 0 ��������
	//    OLEDwrite_comm(7);           // �� 7 ��������
	
	//    OLEDwrite_comm(0x21);        // ������ ������� �� X
	//   OLEDwrite_comm(0);           // �� 0 �������
	//    OLEDwrite_comm(127);         // �� 127 �������
	
	I2C_SendByte(COM);
	I2C_SendByte(0xAF); //������� ������� � ���������� ������

	I2C_StopCondition(); //����
}

//*************** ������� ��������� ������� ******************************
void displaySetPosition(char x, char y)
{
	if((y >= 0) && (y <= 7)) // ���� � ��������� 
	{
		ssd1306_send_com(y+0xB0); //��� �� �������� ������� ������� 7- 0, 6- 1 � �. �.(7-y+0xB0)
	}
	ssd1306_send_com(((x & 0xF0) >> 4) | 0x10);
	ssd1306_send_com((x & 0x0F) | 0x01);
}

//*************** ������� ������� ������� **********************************

void displayClear(void)
{
	displaySetPosition(0,7); //���������� �� ������� �������
	I2C_StartCondition();
	I2C_SendByte(SSD1306_adress); //�������� ������ ����������
	I2C_SendByte(DATS);
	for (int n = 0; n < 1024; ++n)
		I2C_SendByte(0x00); //��������� ��� ������������ ������
	I2C_StopCondition(); //����
}

//*************** ������� ������� ����� ������ **********************************
void displayClearString(char numString){
	displaySetPosition(0,numString); //���������� �� ������
	I2C_StartCondition();
	I2C_SendByte(SSD1306_adress); //�������� ������ ����������
	I2C_SendByte(DATS);
	for (char n = 0; n < 128; ++n)
	I2C_SendByte(0x00); //��������� ��� ������������ ������
	I2C_StopCondition(); //����
}

//************	������� ��������� ������� (������ �� ��������� ��������) *********************

void displayGotoPosition(char x, char y)
{
	if(x > 15) x = 21;
	if(y > 7) y = 7;
	displaySetPosition((x*6), y);
}

//**************** ������� ������ ������� **************************************************

void displayShowChar(char chr)
{	
	if ((chr < 0x20)||((chr > 0x7E)&&(chr < 0xC0)))
			chr = 0x00; //������� ���� ������ 0x00 � ������� ��� ��� ������ 
	else if ((chr >= 0x20)&&(chr <= 0x7E))
			chr -= 0x20; //���� ���� ������� ������ 0x20 (������) �� ������� ASCII � �� 0x7E(������)
			else chr -= 0x61; //��� ������ �������� (�������� � ����� ������� ������)
	
	 	for(char i=0; i<5; i++)
				 //������� ��� ������ (����� ����� ���������� �� 5 ������� � ������� ������ �����)  
	 			//ssd1306_send_data(font6x8[(chr*5)+i]); //��������� ������ ������� ������
				 ssd1306_send_data(EEPROM_read_word((chr*5)+i)); //����� ����� EEPROM
	ssd1306_send_data(0x00); //��������� ������ �������
}
 
//******************* ������� ������ ������ �� �������************************************************

void displayShowStr(char x, char y, char *str) // ������ ���������� � ����� �����
{
	displayGotoPosition(x, y); //������������� ������ 
	char length = 0; //������� ���������� ��������
	while(*str)
	{
		if (length >= 21) //���� ������ 21 ������ � ������
		{
			if(y >= 7) //��������� �� ��������� �� ��� ������
			displayGotoPosition(0, y = 0); //���� �������� ������ ��������� �� ������� ������
			else displayGotoPosition(0, ++y); //���� �� ��������� ������ ��������� �� ��������� ������ � ����� �� ������ 
			length = 0; //���������� ������� ����� ������� �� ����� ������
		}
		displayShowChar(*str); //������� ������
		str++;
		++length;
	}
}

//******************* ������� ������ ������ ************************************************

void displayShowString(char *str) // ������ ���������� � ����� �����
{
	while(*str)
	{
		displayShowChar(*str); //������� ������
		str++;
	}
}
//*******************************	������� ������ ������ ������� ****************************

//void ssd136_symbol(char *arr, char light)
//{
	//char i = 0;
	//
	//for(i=0; i<light; i++)
	//{
		//ssd1306_send_data(*arr);
		//arr++;
	//}
//}
