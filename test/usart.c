#include "usart.h"

void USART_Init (unsigned int speed)//Инициализация модуля USART

{
UBRRH = (unsigned char)(speed>>8);
UBRRL = (unsigned char)speed;	
UCSRB=(1<<RXEN)|( 1<<TXEN); //Включаем прием и передачу по USART
//UCSRB |= (1<<RXCIE); //Разрешаем прерывание при передаче
UCSRA |= (1<<U2X); // Для 8 мгц
UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);// Обращаемся именно к регистру UCSRC (URSEL=1),
//ассинхронный режим (UMSEL=0), без контроля четности (UPM1=0 и UPM0=0),
//1 стоп-бит (USBS=0), 8-бит посылка (UCSZ1=1 и UCSZ0=1)
//UCSRC |= (1<<UPM1);//четность
//UCSRC = (1<<URSEL)|(1<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);БЫЛО 2 СТП БИТА СТАЛО 1
}

void USART_Transmit(unsigned char data) //Функция отправки данных
{
	while ( !(UCSRA & (1<<UDRE)) ); //Ожидание опустошения буфера приема
	UDR = data; //Начало передачи данных

}

unsigned char USART_Receive(void) //функция приема данных
{
	while ( !(UCSRA & (1<<RXC)) ); //ожидание когда в приемном буфере есть непрочитанные данные.
	return UDR; //возврат данных  с буфера
}

void USART_reading_in_array (unsigned char * data, char length) //функция чтения и записи в массив назначенной длины
{
	for (char i = 0; i< length; ++i) //в цикле присваиваем приходящие данные массиву data
	{
		while (!(UCSRA & (1<<RXC))); //ожидание когда в приемном буфере есть непрочитанные данные.
		data [i]= UDR;
	}
}