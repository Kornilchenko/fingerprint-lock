#ifndef SSD_1306_H_
#define SSD_1306_H_

#include "main.h"

// Функция передачи команды
void ssd1306_send_com(char com);
// Функция передачи команды
void ssd1306_send_data(char data);
// Функция инициализации дисплея.
void display_128x64_Init(void);
// Функция установки позиции
void displaySetPosition(char x, char y);
// Функция очистки дисплея
void displayClear(void);
// Функция очистки одной строки
void displayClearString(char numString);
// Функция установки курсора
void displayGotoPosition(char x, char y);
// Функция вывода символа
void displayShowChar(char chr);
//Функция вывода строки на позицию
void displayShowStr(char x, char y, char *str);
//Функция вывода строки
void displayShowString(char *str);

// Функция вывода своего символа
//void ssd136_symbol(char *arr, char light);

#endif /* SSD_1306_H_ */