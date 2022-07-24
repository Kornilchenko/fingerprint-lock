#ifndef EEPROM_H_
#define EEPROM_H_

/*
0-795 -массив символов и букв кирилицы латыницы
800-815 -Данные с АЦП
820-821 - таймер двери
822-823 -таймер меню
824-825 -пароль меню
826 - данные о включении отключении таймера двери
*/


#include "main.h"

void EEPROM_write(unsigned int uiAddress, unsigned char ucData);// пишем
unsigned char EEPROM_read(unsigned int uiAddress);// читаем
void EEPROM_write_word(unsigned int uiAddress, uint16_t ucData);//пишем 2-х байтную величину
uint16_t EEPROM_read_word(unsigned int uiAddress);//читаем 2-х байтную величину
void EEPROM_write_dword(unsigned int uiAddress, uint32_t ucData);//пишем 4-х байтовую величину
uint32_t EEPROM_read_dword(unsigned int uiAddress); // читаем 4-х байтовую величину
void EEPROM_write_string(unsigned int uiAddress, char str1[]);// пишем строку
const char* EEPROM_read_string(unsigned int uiAddress, unsigned int sz);// читаем строку


#endif