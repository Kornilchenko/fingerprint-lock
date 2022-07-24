#include "lcd.h"

void sendhalfbyte(unsigned char c)
{
	c<<=4;//сдвинем наш входной аргумент влево на 4 бита, так как работаем мы со старшими разрядами шины (4-7)
	e1; //включаем линию Е
	_delay_us(50);
	PORTA&=0b00001111; //стираем информацию на входах DB4-DB7, остальное не трогаем
	PORTA|=c;
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
void setpos(unsigned char x, unsigned y)
{
	char adress;
	adress=(0x40*y+x)|0b10000000;
	sendbyte(adress, 0);
}
//------------------------------
void LCD_ini(void)
{
	_delay_ms(100);
	sendhalfbyte(0b00000011);
	_delay_us(40);
	sendhalfbyte(0b00000010);
	sendhalfbyte(0b00001000);//дисплей в 2 линии (N=1), режим 4 бит(DL=0)
	_delay_us(40);
	sendhalfbyte(0b00000010);
	sendhalfbyte(0b00001000);
	_delay_us(40);
	sendbyte(0b00001100, 0); //включаем изображение на дисплее (D=1), курсоры никакие не включаем (C=0, B=0)
	_delay_ms(1);
	sendbyte(0b00000001, 0); //display clear
	_delay_ms(20);// было 1
	sendbyte(0b00000110, 0); //курсор (хоть он у нас и невидимый) будет двигаться влево
	_delay_ms(1);
	
}
//-----------------------------------------------
void clearlcd()// очистка дисплея
{
sendbyte(0b00000001, 0); //display clear
_delay_ms(2);	
}
//-----------------------------------------------
void str_lcd (char str1[])
{
	uint8_t n;
	//wchar_t n;
	for(n=0;str1[n]!='\0';n++) sendchar(str1[n]);
}