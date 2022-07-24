#ifndef USART_H_
#define USART_H_

#include "main.h"

void USART_Init (unsigned int speed); //Инициализация модуля USART
void USART_Transmit(unsigned char data); //Функция отправки данных
unsigned char USART_Receive(void); //функция приема данных
void USART_reading_in_array (unsigned char * data, char length); //функция чтения и записи в массив назначенной длины

#endif /* USART_H_ */