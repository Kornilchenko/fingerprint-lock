#include "lcd_I2C.h"
unsigned char portlcd = 0; //ячейка для хранения данных порта микросхемы расширения

void sendhalfbyte(unsigned char c)
{
	c<<=4;//сдвинем наш входной аргумент влево на 4 бита, так как работаем мы со старшими разрядами шины (4-7)
	e1; //включаем линию Е
	_delay_us(50);
	//I2C_SendByteByADDR(portlcd|c,0b01001110);
	I2C_SendByteByADDR(portlcd|c,0b01111110);
	e0; //выключаем линию Е
	_delay_us(50);

}
//------------------------------
void sendbyte(unsigned char c, unsigned char mode)
{
	if (mode==0) rs0;
	else rs1;
	unsigned char hc=0;
	hc=c>>4;
	sendhalfbyte(hc); sendhalfbyte(c);
}
//------------------------------
void sendchar(unsigned char c)//функция, которая будет передавать только данные, а не команду
{
	sendbyte(c,1);
}
//------------------------------
void setpos(unsigned char x, unsigned y) //позиция начала строки
{
	char adress;
	adress=(0x40*y+x)|0b10000000;
	sendbyte(adress, 0);
}
//------------------------------
void LCD_ini(void)// инициализация дисплея
{
	_delay_ms(20);
	sendhalfbyte(0b00000011);
	_delay_ms(4);
	sendhalfbyte(0b00000011);
	_delay_us(100);
	sendhalfbyte(0b00000011);
	_delay_ms(1);
	sendhalfbyte(0b00000010);
	_delay_ms(1);
	sendbyte(0b00101000, 0); //4бит-режим (DL=0) и 2 линии (N=1)
	_delay_ms(1);
	sendbyte(0b00001100, 0); //включаем изображение на дисплее (D=1), курсоры никакие не включаем (C=0, B=0)
	_delay_ms(1);
	sendbyte(0b00000110, 0); //курсор (хоть он у нас и невидимый) будет двигаться влево
	_delay_ms(1);
	setled();//подсветка
	setwrite();//запись
}
//-----------------------------------------------
void clearlcd()// очистка дисплея
{
sendbyte(0b00000001, 0); //display clear
_delay_ms(20);	
}
//-----------------------------------------------
void str_lcd (char str1[])
{
	uint8_t n;
	for(n=0;str1[n]!='\0';n++)
	 sendchar(str1[n]);
}