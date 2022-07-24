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
#define is_button_presed !(PINC&button) //кнопка нажата

//********************закрытие замка*********************************
void closeLock(){
	rele_close_door_on; 
	_delay_ms(300);
	rele_close_door_off;
}

//*********************открытие замка*********************************
void openLock(){
	rele_open_door_on;
	_delay_ms(300);
	rele_open_door_off;
}

//****************опрос кнопки внутри квартиры и результат по нажатию этой кнопки
void workInnerButton(){
	if(is_button_presed && is_closed_door){ //если кнопка в квартире нажата, если дверь закрыта
		while (is_button_presed); //ждем отпускания кнопки
		if(is_closed_lock) //если замок закрыт
			openLock();
		else
			closeLock();	
	}
}

//*******************проверка состояния датчика замка двери ************
void checkLock(){
	if(is_closed_lock){ //если замок закрыт
		displayShowStr(0,1, "Замок закрыт");
		LED_open_off;
		LED_close_on;
		}else{
		displayShowStr(0,1, "Замок открыт");
		LED_close_off;
		LED_open_on;
	}
}

//********************проверка состояния двери*********************
void checkDoor(){
	if(is_open_door)
		displayShowStr(0,0,"Дверь открыта");
	else
		displayShowStr(0,0,"Дверь закрыта");	
}

 unsigned int timeCloseDoor; //*****таймер закрытия двери*****
 
 char doorTimerState; //****состояние таймера двери вкл/откл
 
 //********чтение уставок для работы двери, инициализация*********
 void initParametrDoor(){
	 timeCloseDoor = readValueTimerCloseDoor();
	 doorTimerState = getStateTimerDoor();
 }
 
 //********считывание состояния таймера двери вкл/откл *********
 char getStateTimerDoor(){
	 return EEPROM_read(826);
 }
 
 //*********включение таймера двери************
 void doorTimerOn(){
	 doorTimerState = 1;
	 EEPROM_write(826,1);
 }
 
 //*********отключение таймера двери************
 void doorTimerOff(){
	 doorTimerState = 0;
	 EEPROM_write(826,0);
 }
 
 //*********считывание данных с памяти о таймере двери************
  unsigned int readValueTimerCloseDoor(){
	  return EEPROM_read_word(820);
  }
  
  //*******запись данных о таймере закрытия двери**********
  void writeValueTimerCloseDoor(unsigned int timeClose){
	  timeCloseDoor = timeClose / 5;
	  EEPROM_write_word(820, timeCloseDoor);
  }
 
//*************установка времени закрытия двери****************
void setTimerCloseDoor(unsigned int timeClose){
	timeCloseDoor = timeClose/5;
	
}

//************Закрываем замок по истечении времени*****************
void closeDoorByTimer(){ 
	if (is_closed_door){ //если дверь закрыта
		if (is_closed_lock) //если замок закрыт
		setTimerCount(0);
		else if (getTimerCount() >= timeCloseDoor){
			rele_close_door_on; //подаем сигнал на закрытие
			_delay_ms(300);
			rele_close_door_off; //снимаем сигнал закрытия
			setTimerCount(0); //обнуляем счетчик
		}
	}else{
		setTimerCount(0); //обнуляем счетчик
	}
}

//**********работа датчиков двери замка в целом****************
void workCloseOpenDoorGeneral(){
	if (doorTimerState)
		closeDoorByTimer();
	 checkDoor();
	 checkLock();
	 workInnerButton();
}
