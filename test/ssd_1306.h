#ifndef SSD_1306_H_
#define SSD_1306_H_

#include "main.h"

// Функция передачи команды
void ssd1306_send_com(char com);
// Функция передачи команды
void ssd1306_send_data(char data);
// Функция инициализации дисплея.
void ssd1306_init(void);
// Функция установки позиции
void ssd1306_position(char x, char y);
// Функция очистки дисплея
void ssd1306_clear(void);
// Функция установки курсора
void ssd1306_goto_xy(char x, char y);
// Функция вывода символа
void ssd1306_putchar(char chr);
//Функция вывода строки
void ssd1306_putstr(char *str);
// Функция вывода своего символа
void ssd136_symbol(char *arr, char light);

#endif /* SSD_1306_H_ */