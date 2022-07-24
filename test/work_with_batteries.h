#ifndef WORK_WITH_BATTERIES_H_
#define WORK_WITH_BATTERIES_H_

#include "main.h"

//****************������������� ������ � ���������****************
void initWorkBattery();
//**********************������ � ��������������*******************
void workWithBatteries();
//*****************************getVoltage*************************
float getVoltage(char numBatery);
//**************** ��������� ������ ������� **********************
char getChargeMode();
//**************** ��������� �������� ****************************
char getBypassMode(char numBatery);
//***************��������� ����������� ***************************
void setCoefficient(float voltage, char numBatery);
//**************��������� ������������ ���������� ������� ********
void setMinimumVoltage(float voltage);
//*************��������� ������������� ���������� ������ *********
void setMaximumVoltage(float voltage);


#endif 