#ifndef ENCODER_H_
#define ENCODER_H_

#include "main.h"

//**************��������� ������ ��������**********************-
char getPressButtonEncoder();
//***********������������� ������ � ���������***************
void initEncoder();
//****************���������� ������ �������� ****************
 char getStateEncoder();
//***********��������� ������ ��������****************
void timerOff();
//***************�������� ������ ��������*****************
void timerOn();

#endif