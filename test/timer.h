#ifndef TIMER_H_
#define TIMER_H_

#include "menu.h"

//***********получение тукущего значения таймера*************
char getTimerCount();

//*************установка текущего значения таймера***************
void setTimerCount(unsigned int num);

//*********************инициализация таймера**************************
void timer_ini(void);


#endif 