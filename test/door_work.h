#ifndef DOOR_WORK_H_
#define DOOR_WORK_H_

#include "main.h"


#define lock_sensor		0b10000000
#define door_sersor		0b01000000
#define is_closed_door PINA&door_sersor //концевой разомкнут
#define is_closed_lock !(PINA&lock_sensor) //концевой замкнут
#define is_open_door !(PINA&door_sersor) //концевой замкнут
#define is_open_lock PINA&lock_sensor //концевой разомкнут

 //********чтение уставок для работы двери, инициализация*********
 void initParametrDoor();

//*********считывание данных с памяти о таймере двери************
unsigned int readValueTimerCloseDoor();
  
//*******запись данных о таймере закрытия двери**********
void writeValueTimerCloseDoor(unsigned int timeClose);
  
//********считывание состояния таймера двери вкл/откл *********
char getStateTimerDoor();

//*********включение таймера двери************
 void doorTimerOn();
 
 //*********отключение таймера двери************
 void doorTimerOff();
 
 //**********работа датчиков двери замка в целом****************
 void workCloseOpenDoorGeneral();

#endif 