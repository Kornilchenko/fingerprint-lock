#ifndef WORK_WITH_BATTERIES_H_
#define WORK_WITH_BATTERIES_H_

#include "main.h"

//****************инициализация работы с батареями****************
void initWorkBattery();
//**********************работа с аккумуляторами*******************
void workWithBatteries();
//*****************************getVoltage*************************
float getVoltage(char numBatery);
//**************** состояние режима зарядки **********************
char getChargeMode();
//**************** состояние байпасов ****************************
char getBypassMode(char numBatery);
//***************установка коефициента ***************************
void setCoefficient(float voltage, char numBatery);
//**************установка минимального напряжения разряда ********
void setMinimumVoltage(float voltage);
//*************установка максимального напряжения заряда *********
void setMaximumVoltage(float voltage);


#endif 