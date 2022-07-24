#ifndef DOOR_WORK_H_
#define DOOR_WORK_H_

#include "main.h"


#define lock_sensor		0b10000000
#define door_sersor		0b01000000
#define is_closed_door PINA&door_sersor //�������� ���������
#define is_closed_lock !(PINA&lock_sensor) //�������� �������
#define is_open_door !(PINA&door_sersor) //�������� �������
#define is_open_lock PINA&lock_sensor //�������� ���������

 //********������ ������� ��� ������ �����, �������������*********
 void initParametrDoor();

//*********���������� ������ � ������ � ������� �����************
unsigned int readValueTimerCloseDoor();
  
//*******������ ������ � ������� �������� �����**********
void writeValueTimerCloseDoor(unsigned int timeClose);
  
//********���������� ��������� ������� ����� ���/���� *********
char getStateTimerDoor();

//*********��������� ������� �����************
 void doorTimerOn();
 
 //*********���������� ������� �����************
 void doorTimerOff();
 
 //**********������ �������� ����� ����� � �����****************
 void workCloseOpenDoorGeneral();

#endif 