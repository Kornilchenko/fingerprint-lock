#include "door_work.h"

#define rele_open_door PC3
#define rele_open_door_on PORTC |=  (1<<rele_open_door)
#define rele_open_door_off PORTC &= ~(1<<rele_open_door)

#define rele_close_door PC2
#define rele_close_door_on PORTC |=  (1<<rele_close_door)
#define rele_close_door_off PORTC &= ~(1<<rele_close_door)

#define LED_open PC4
#define LED_open_on PORTC |=  (1<<LED_open)
#define LED_open_off PORTC &= ~(1<<LED_open)

#define LED_close PC5
#define LED_close_on PORTC |=  (1<<LED_close)
#define LED_close_off PORTC &= ~(1<<LED_close)

#define button		0b01000000
#define is_button_presed !(PINC&button) //������ ������

//********************�������� �����*********************************
void closeLock(){
	rele_close_door_on; 
	_delay_ms(300);
	rele_close_door_off;
}

//*********************�������� �����*********************************
void openLock(){
	rele_open_door_on;
	_delay_ms(300);
	rele_open_door_off;
}

//****************����� ������ ������ �������� � ��������� �� ������� ���� ������
void workInnerButton(){
	if(is_button_presed && is_closed_door){ //���� ������ � �������� ������, ���� ����� �������
		while (is_button_presed); //���� ���������� ������
		if(is_closed_lock) //���� ����� ������
			openLock();
		else
			closeLock();	
	}
}

//*******************�������� ��������� ������� ����� ����� ************
void checkLock(){
	if(is_closed_lock){ //���� ����� ������
		displayShowStr(0,1, "����� ������");
		LED_open_off;
		LED_close_on;
		}else{
		displayShowStr(0,1, "����� ������");
		LED_close_off;
		LED_open_on;
	}
}

//********************�������� ��������� �����*********************
void checkDoor(){
	if(is_open_door)
		displayShowStr(0,0,"����� �������");
	else
		displayShowStr(0,0,"����� �������");	
}

 unsigned int timeCloseDoor; //*****������ �������� �����*****
 
 char doorTimerState; //****��������� ������� ����� ���/����
 
 //********������ ������� ��� ������ �����, �������������*********
 void initParametrDoor(){
	 timeCloseDoor = readValueTimerCloseDoor();
	 doorTimerState = getStateTimerDoor();
 }
 
 //********���������� ��������� ������� ����� ���/���� *********
 char getStateTimerDoor(){
	 return EEPROM_read(826);
 }
 
 //*********��������� ������� �����************
 void doorTimerOn(){
	 doorTimerState = 1;
	 EEPROM_write(826,1);
 }
 
 //*********���������� ������� �����************
 void doorTimerOff(){
	 doorTimerState = 0;
	 EEPROM_write(826,0);
 }
 
 //*********���������� ������ � ������ � ������� �����************
  unsigned int readValueTimerCloseDoor(){
	  return EEPROM_read_word(820);
  }
  
  //*******������ ������ � ������� �������� �����**********
  void writeValueTimerCloseDoor(unsigned int timeClose){
	  timeCloseDoor = timeClose / 5;
	  EEPROM_write_word(820, timeCloseDoor);
  }
 
//*************��������� ������� �������� �����****************
void setTimerCloseDoor(unsigned int timeClose){
	timeCloseDoor = timeClose/5;
	
}

//************��������� ����� �� ��������� �������*****************
void closeDoorByTimer(){ 
	if (is_closed_door){ //���� ����� �������
		if (is_closed_lock) //���� ����� ������
		setTimerCount(0);
		else if (getTimerCount() >= timeCloseDoor){
			rele_close_door_on; //������ ������ �� ��������
			_delay_ms(300);
			rele_close_door_off; //������� ������ ��������
			setTimerCount(0); //�������� �������
		}
	}else{
		setTimerCount(0); //�������� �������
	}
}

//**********������ �������� ����� ����� � �����****************
void workCloseOpenDoorGeneral(){
	if (doorTimerState)
		closeDoorByTimer();
	 checkDoor();
	 checkLock();
	 workInnerButton();
}
