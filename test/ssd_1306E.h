#ifndef SSD_1306_H_
#define SSD_1306_H_

#include "main.h"

// ������� �������� �������
void ssd1306_send_com(char com);
// ������� �������� �������
void ssd1306_send_data(char data);
// ������� ������������� �������.
void display_128x64_Init(void);
// ������� ��������� �������
void displaySetPosition(char x, char y);
// ������� ������� �������
void displayClear(void);
// ������� ������� ����� ������
void displayClearString(char numString);
// ������� ��������� �������
void displayGotoPosition(char x, char y);
// ������� ������ �������
void displayShowChar(char chr);
//������� ������ ������ �� �������
void displayShowStr(char x, char y, char *str);
//������� ������ ������
void displayShowString(char *str);

// ������� ������ ������ �������
//void ssd136_symbol(char *arr, char light);

#endif /* SSD_1306_H_ */