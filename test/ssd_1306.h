#ifndef SSD_1306_H_
#define SSD_1306_H_

#include "main.h"

// ������� �������� �������
void ssd1306_send_com(char com);
// ������� �������� �������
void ssd1306_send_data(char data);
// ������� ������������� �������.
void ssd1306_init(void);
// ������� ��������� �������
void ssd1306_position(char x, char y);
// ������� ������� �������
void ssd1306_clear(void);
// ������� ��������� �������
void ssd1306_goto_xy(char x, char y);
// ������� ������ �������
void ssd1306_putchar(char chr);
//������� ������ ������
void ssd1306_putstr(char *str);
// ������� ������ ������ �������
void ssd136_symbol(char *arr, char light);

#endif /* SSD_1306_H_ */