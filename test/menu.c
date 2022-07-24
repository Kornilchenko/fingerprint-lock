#include "menu.h"

char menuItem; //пункт меню

#define NO_ACTION			0									
#define LONG_PRESS_BUTTON	1
#define PRESS_BUTTON		2
#define ROTATE_LEFT			3
#define ROTATE_RIGHT		4

#define FINGERPRINTS		1 //отпечатки
#define ADC					2 //АЦП
#define SET_TIMER			3 //таймеры
#define MENU_PASSWORD		4 //пароль меню
#define EXIT				5 //выход
#define MENU_ITEMS			6 //количество пунктов меню

char enkoderState; //состояние энкодера
	
//******главное меню настроек*********
void showMenuGeneral(){
	menuItem = 1;
	setTimerCount(0); //установка таймера в 0
	displayClear();
	timerOn();
	if(changeMenuPassword() == 0){ //проверяем пароль
		displayClear();
		timerOff();
		return;
	}
	
	while(getTimerCount() != 3){
		displayShowStr(8,0,"МЕНЮ");
		displayShowStr(2,1,"Меню отпечатков");
		displayShowStr(2,2,"Меню АЦП");
		displayShowStr(2,3,"Настройки таймера");
		displayShowStr(2,4,"Смена пароля");
		displayShowStr(2,5,"ВЫХОД");
		displayShowStr(0,menuItem,"*");
		timerOff();
		enkoderState = getStateEncoder();
		timerOn();
		if(enkoderState == PRESS_BUTTON){
			setTimerCount(0); //установка таймера в 0
			if(menuItem == FINGERPRINTS){
				 showMenuFingerprints();
				 menuItem = FINGERPRINTS;
			}
			if(menuItem == ADC) {
				showMenuADC();
				menuItem = ADC;
			}
			if(menuItem == SET_TIMER){
				 showMenuSetingTimer();
				 menuItem = SET_TIMER;
			}
			if(menuItem == MENU_PASSWORD){
				showPasswordMenu();
				menuItem = MENU_PASSWORD;
			}
			if(menuItem == EXIT){
				timerOff(); //exit
				displayClear();
				return;
			}
		}
		if(enkoderState == ROTATE_LEFT && menuItem > 1){
			setTimerCount(0); //установка таймера в 0
			displayShowStr(0, menuItem, " ");
			--menuItem;
			displayShowStr(0, menuItem, "*");
		}
		if(enkoderState == ROTATE_RIGHT && menuItem < MENU_ITEMS - 1){
			setTimerCount(0); //установка таймера в 0
			displayShowStr(0, menuItem, " ");
			++menuItem;
			displayShowStr(0, menuItem, "*");
		}
		_delay_ms(10);
	}
	displayClear();
	timerOff();
}

//********меню настроек отпечатков*******
void showMenuFingerprints(void){
	menuItem = 1;
	setTimerCount(0); //установка таймера в 0
	displayClear();
	while(getTimerCount() != 3){
		displayShowStr(0, menuItem, "*");
		displayShowStr(3,0,"МЕНЮ ОТПЕЧАТКОВ");
		displayShowStr(2,1,"Добавить отпечаток");
		displayShowStr(2,2,"Удалить отпечатки");
		displayShowStr(2,3,"Изменить пароль");
		displayShowStr(2,4,"ВЫХОД");
		timerOff();
		enkoderState = getStateEncoder();
		timerOn();
		if(enkoderState == PRESS_BUTTON){
			setTimerCount(0); //установка таймера в 0
			if(menuItem == 1){
				addingFingerInLibrary();
				menuItem = 1;
			}
			if(menuItem == 2) {
				deliteFingerInLibrary();
				menuItem = 2;
			}
			if(menuItem == 3){
				//showMenuSetingTimer();
				menuItem = 3;
			}
			if(menuItem == 4){
				displayClear();
				return;
			}
		}else if(enkoderState == ROTATE_LEFT && menuItem > 1){
			setTimerCount(0); //установка таймера в 0
			displayShowStr(0, menuItem, " ");
			--menuItem;
			displayShowStr(0, menuItem, "*");
		}else if(enkoderState == ROTATE_RIGHT && menuItem < 4){
			setTimerCount(0); //установка таймера в 0
			displayShowStr(0, menuItem, " ");
			++menuItem;
			displayShowStr(0, menuItem, "*");
		}else if(enkoderState == LONG_PRESS_BUTTON){
			setTimerCount(0); //установка таймера в 0
			displayClear();
			return;
		}
		_delay_ms(10);
	}
	displayClear();
}

//**********меню настроек АЦП**********
void showMenuADC(void){
	menuItem = 1;
	setTimerCount(0); //установка таймера в 0
	displayClear();
	while(getTimerCount() != 3){
		displayShowStr(0, menuItem, "*");
		displayShowStr(4,0,"МЕНЮ АЦП");
		displayShowStr(2,1,"Напряжение разряда");
		displayShowStr(2,2,"Hапряжение заряда");
		displayShowStr(2,3,"Калибровать БАТ1");
		displayShowStr(2,4,"Калибровать БАТ2");
		displayShowStr(2,5,"Калибровать БАТ3");
		displayShowStr(2,6,"ВЫХОД");
		timerOff();
		enkoderState = getStateEncoder();
		timerOn();
		if(enkoderState == PRESS_BUTTON){
			setTimerCount(0); //установка таймера в 0
			if(menuItem == 1){
				menuSetMinimumVoltage();
				menuItem = 1;
			}
			if(menuItem == 2) {
				menuSetMaximumVoltage();
				menuItem = 2;
			}
			if(menuItem == 3){
				calibrationBat1();
				menuItem = 3;
			}
			if(menuItem == 4){
				calibrationBat2();
				menuItem = 4;
			}
			if(menuItem == 5){
				calibrationBat3();
				menuItem = 5;
			}
			if(menuItem == 6){
				displayClear();
				return;
			}
		}else if(enkoderState == ROTATE_LEFT && menuItem > 1){
			setTimerCount(0); //установка таймера в 0
			displayShowStr(0, menuItem, " ");
			--menuItem;
			displayShowStr(0, menuItem, "*");
		}else if(enkoderState == ROTATE_RIGHT && menuItem < 6){
			setTimerCount(0); //установка таймера в 0
			displayShowStr(0, menuItem, " ");
			++menuItem;
			displayShowStr(0, menuItem, "*");
		}else if(enkoderState == LONG_PRESS_BUTTON){
			setTimerCount(0); //установка таймера в 0
			displayClear();
			return;
		}
		_delay_ms(10);
	}
	displayClear();
}

//**********меню настроек таймера**********
void showMenuSetingTimer(void){
	menuItem = 1;
	setTimerCount(0); //установка таймера в 0
	displayClear();
	while(getTimerCount() != 3){
		displayShowStr(0, menuItem, "*");
		displayShowStr(1,0,"НАСТРОЙКА ТАЙМЕРА");
		displayShowStr(2,1,"Таймер на дверь");
		displayShowStr(2,2,"Таймер двери вкл");
		displayShowStr(2,3,"Таймер двери выкл");
		displayShowStr(2,4,"ВЫХОД");
		timerOff();
		enkoderState = getStateEncoder();
		timerOn();
		if(enkoderState == PRESS_BUTTON){
			setTimerCount(0); //установка таймера в 0
			if(menuItem == 1){
				setTimerDoorMenu();
				menuItem = 1;
			}
			if(menuItem == 2) {
				doorTimerOn();
				displayClear();
				displayShowStr(0,3,"Таймер включен");
				_delay_ms(2000);
				displayClear();
			}
			if(menuItem == 3){
				doorTimerOff();
				displayClear();
				displayShowStr(0,3,"Таймер отключен");
				_delay_ms(2000);
				displayClear();
			}
			if(menuItem == 4){
				displayClear();
				return;
			}
		}else if(enkoderState == ROTATE_LEFT && menuItem > 1){
			setTimerCount(0); //установка таймера в 0
			displayShowStr(0, menuItem, " ");
			--menuItem;
			displayShowStr(0, menuItem, "*");
		}else if(enkoderState == ROTATE_RIGHT && menuItem < 4){
			setTimerCount(0); //установка таймера в 0
			displayShowStr(0, menuItem, " ");
			++menuItem;
			displayShowStr(0, menuItem, "*");
		}else if(enkoderState == LONG_PRESS_BUTTON){
			setTimerCount(0); //установка таймера в 0
			displayClear();
			return;
		}
		_delay_ms(10);
	}
	displayClear();
}

//*******проверка пароля*************
char changeMenuPassword(){
	setTimerCount(0); //установка таймера в 0
	displayClear();
	unsigned char num = 0;
	unsigned int password = 0;
	char str[5];
	
	//для отладки
	unsigned int pas = EEPROM_read_word(824);
	sprintf(str, "%d", pas);
	displayShowStr(8, 6, str);
	
	displayShowStr(3,0,"введите пароль");
	displayShowStr(8,1,"000");
	for (char i=0; i<3; ){ //установка пароля
		if(getTimerCount() == 3) return 0; //выход по времени
		sprintf(str, "%d", num);
		displayShowStr(8+i, 1, str);
		displayShowStr(7+i, 2, " "); //затираем звёздочку
		displayShowStr(8+i, 2, "*"); //звездочка под набираемым символом
		
		timerOff();
		enkoderState = getStateEncoder();
		timerOn();
		if(enkoderState == ROTATE_LEFT && num > 0){
			setTimerCount(0); //установка таймера в 0
			--num;
			}else if(enkoderState == ROTATE_RIGHT && num < 9){
			setTimerCount(0); //установка таймера в 0
			++num;
			}else if(enkoderState == PRESS_BUTTON){
			setTimerCount(0); //установка таймера в 0
			if(i == 0) password += num * 100;
			else if(i == 1) password += num * 10;
			else if(i == 2) password += num;
			++i;
			num = 0;
		}
		_delay_ms(20);
	}
	char result = 0;
	displayClear();
	if(pas == password){
		 displayShowStr(3,3,"Пароль верный");
		 result = 1;
	}else{
		 displayShowStr(3,3,"Пароль не верный");
	}
	_delay_ms(700);
	displayClear();
	return result;
}

//*********меню смены пароля************
void showPasswordMenu(){
	setTimerCount(0); //установка таймера в 0
	displayClear();
	unsigned char num1 = 0;
	char flag = 1;
	unsigned int num = 0;
	char str[5];
	displayShowStr(3,0,"установить пароль");
	displayShowStr(8,1,"000");
	for (char i=0; i<3; ){ //установка пароля
		if(getTimerCount() == 3) return; //выход по времени
		sprintf(str, "%d", num1);
		displayShowStr(8+i, 1, str);
		displayShowStr(7+i, 2, " "); //затираем звёздочку
		displayShowStr(8+i, 2, "*"); //звездочка под набираемым символом
		timerOff();
		enkoderState = getStateEncoder();
		timerOn();
		if(enkoderState == ROTATE_LEFT && num1 > 0){
			setTimerCount(0); //установка таймера в 0
			--num1;	
		}else if(enkoderState == ROTATE_RIGHT && num1 < 9){
			setTimerCount(0); //установка таймера в 0
			++num1;
		}else if(enkoderState == PRESS_BUTTON){
			setTimerCount(0); //установка таймера в 0
			if(i == 0) num += num1 * 100;
			else if(i == 1) num += num1 * 10;
			else if(i == 2) num += num1;
			++i;
			num1 = 0;
		}
		_delay_ms(20);
	}
	sprintf(str, "%d", num);
	displayShowStr(8, 1, str); //выводим пароль
	displayClearString(2); //стираем строку со звёздочкой
	displayShowStr(2, 3, "Сохранить пароль?");
	displayShowStr(4, 4, "Да?");
	displayShowStr(12, 4, "Нет?");
	while(getTimerCount() != 3){ 
		if(flag){
			displayShowStr(3, 4, "*");
			displayShowStr(11, 4, " ");
		}else{
			displayShowStr(3, 4, " ");
			displayShowStr(11, 4, "*");
		}
		timerOff();
		enkoderState = getStateEncoder();
		timerOn();
		
		if(enkoderState == ROTATE_LEFT){
			setTimerCount(0); //установка таймера в 0
			if (flag) flag=0;
			else flag=1;
		}else if(enkoderState == ROTATE_RIGHT){
			setTimerCount(0); //установка таймера в 0
			if (flag) flag=0;
			else flag=1;
		}else if(enkoderState == PRESS_BUTTON){
			setTimerCount(0); //установка таймера в 0
			displayClear();
			if (flag){
				EEPROM_write_word(824, num);
				displayShowStr(2, 3, "пароль сохранен"); 	
			}else{
				displayShowStr(2, 3, "пароль не сохранен");
			}
			_delay_ms(2000);
			displayClear();
			return;
		}
		_delay_ms(20);
	}
	displayClear();
}

//*********установка минимального напряжения***************
void menuSetMinimumVoltage(){
	setTimerCount(0); //установка таймера в 0
	displayClear();
	displayShowStr(0,0,"Конечное напряжение");
	displayShowStr(6,1,"разряда");
	char voltageArray [5];
	float minimumVoltage =  EEPROM_read(812) + (float)(EEPROM_read(813)/100.0);
	while(getTimerCount() != 3){
		sprintf(voltageArray, "%.2f", minimumVoltage);
		displayShowStr(7, 3, voltageArray );
		timerOff();
		enkoderState = getStateEncoder();
		timerOn();
		if(enkoderState == ROTATE_LEFT){
			setTimerCount(0); //установка таймера в 0
			minimumVoltage -= 0.01;
		}else if(enkoderState == ROTATE_RIGHT){
			setTimerCount(0); //установка таймера в 0
			minimumVoltage += 0.01;
		}else if(enkoderState == PRESS_BUTTON){
			setMinimumVoltage(minimumVoltage);
			displayClear();
			return;
		}else if(enkoderState == LONG_PRESS_BUTTON){
			setTimerCount(0); //установка таймера в 0
			displayClear();
			return;
		}
		_delay_ms(20);
	}
	displayClear();
}

//*********установка максимального напряжения**************
void menuSetMaximumVoltage(){
	setTimerCount(0); //установка таймера в 0
	displayClear();
	displayShowStr(0,0,"Конечное напряжение");
	displayShowStr(6,1,"заряда");
	char voltageArray [5];
	float maximumVoltage = EEPROM_read(814) + (float)(EEPROM_read(815)/100.0);
	while(getTimerCount() != 3){
		sprintf(voltageArray, "%.2f", maximumVoltage);
		displayShowStr(6, 3, voltageArray );
		timerOff();
		enkoderState = getStateEncoder();
		timerOn();
		if(enkoderState == ROTATE_LEFT){
			setTimerCount(0); //установка таймера в 0
			maximumVoltage -= 0.01;
		}else if(enkoderState == ROTATE_RIGHT){
			setTimerCount(0); //установка таймера в 0
			maximumVoltage += 0.01;
		}else if(enkoderState == PRESS_BUTTON){
			setTimerCount(0); //установка таймера в 0
			setMaximumVoltage(maximumVoltage);
			displayClear();
			return;
		}else if(enkoderState == LONG_PRESS_BUTTON){
			setTimerCount(0); //установка таймера в 0
			displayClear();
			return;
		}
		_delay_ms(20);
	}
	displayClear();	
}

//*********калибровка напряжения батареи 1*************
void calibrationBat1(){
	setTimerCount(0); //установка таймера в 0
	displayClear();
	displayShowStr(3,0,"напряжение бат1");
	char voltageArray [5];
	float voltage = getVoltage(1);
	while(getTimerCount() != 3){
		sprintf(voltageArray, "%.2f", voltage);
		displayShowStr(6, 1, voltageArray );
		timerOff();
		enkoderState = getStateEncoder();
		timerOn();
		if(enkoderState == ROTATE_LEFT){
			setTimerCount(0); //установка таймера в 0
			voltage -= 0.01;
		}else if(enkoderState == ROTATE_RIGHT){
			setTimerCount(0); //установка таймера в 0
			voltage += 0.01;
		}else if(enkoderState == PRESS_BUTTON){
			setTimerCount(0); //установка таймера в 0
			setCoefficient(voltage, 1);
			initWorkBattery();
			displayClear();
			return;
		}else if(enkoderState == LONG_PRESS_BUTTON){
			setTimerCount(0); //установка таймера в 0
			displayClear();
			return;
		}
		_delay_ms(20);
	}	
	displayClear();
}

//*********калибровка напряжения батареи 2*************
void calibrationBat2(){
	setTimerCount(0); //установка таймера в 0
	displayClear();
	displayShowStr(3,0,"напряжение бат2");
	char voltageArray [5];
	float voltage = getVoltage(2);
	while(getTimerCount() != 3){
		sprintf(voltageArray, "%.2f", voltage);
		displayShowStr(6, 1, voltageArray );
		timerOff();
		enkoderState = getStateEncoder();
		timerOn();
		if(enkoderState == ROTATE_LEFT){
			setTimerCount(0); //установка таймера в 0
			voltage -= 0.01;
		}else if(enkoderState == ROTATE_RIGHT){
			setTimerCount(0); //установка таймера в 0
			voltage += 0.01;
		}else if(enkoderState == PRESS_BUTTON){
			setTimerCount(0); //установка таймера в 0
			setCoefficient(voltage, 2);
			initWorkBattery();
			displayClear();
			return;
		}else if(enkoderState == LONG_PRESS_BUTTON){
		setTimerCount(0); //установка таймера в 0
		displayClear();
		return;
		}
		_delay_ms(20);
	}
	displayClear();	
}

//*********калибровка напряжения батареи 3*************
void calibrationBat3(){
	setTimerCount(0); //установка таймера в 0
	displayClear();
	displayShowStr(3,0,"напряжение бат3");
	char voltageArray [5];
	float voltage = getVoltage(3);
	while(getTimerCount() != 3){
		sprintf(voltageArray, "%.2f", voltage);
		displayShowStr(6, 1, voltageArray );
		timerOff();
		enkoderState = getStateEncoder();
		timerOn();
		if(enkoderState == ROTATE_LEFT){
			setTimerCount(0); //установка таймера в 0
			voltage -= 0.01;
		}else if(enkoderState == ROTATE_RIGHT){
			setTimerCount(0); //установка таймера в 0
			voltage += 0.01;
		}else if(enkoderState == PRESS_BUTTON){
			setTimerCount(0); //установка таймера в 0
			setCoefficient(voltage, 3);
			initWorkBattery();
			displayClear();
			return;
		}else if(enkoderState == LONG_PRESS_BUTTON){
			setTimerCount(0); //установка таймера в 0
			displayClear();
			return;
		}
		_delay_ms(20);
	}	
	displayClear();
}

//****************добавление отпечатка*****************
void addingFingerInLibrary(){
	setTimerCount(0); //установка таймера в 0
	displayClear();
	displayShowStr(3,0,"добавить отпечаток");
	displayShowStr(0,1, "в библиотеку");
	char liblary = 1;
	char array[1];
	char result;
	while(getTimerCount() != 3){
		sprintf(array, "%d", liblary);
		displayShowStr(13,1, array);
		timerOff();
		enkoderState = getStateEncoder();
		timerOn();
		if(enkoderState == ROTATE_LEFT && liblary > 1){
			setTimerCount(0); //установка таймера в 0
			--liblary;
		}else if(enkoderState == ROTATE_RIGHT && liblary < 3){
			setTimerCount(0); //установка таймера в 0
			++liblary;
		}else if(enkoderState == PRESS_BUTTON){
			setTimerCount(0); //установка таймера в 0
			result = addingFingerR300(liblary);
			if(result)
				displayShowStr(0,3,"удачно добавлен  ");
			else
				displayShowStr(0,3,"ошибка добавления");
			_delay_ms(1000);
			displayClear();
			return;
		}else if(enkoderState == LONG_PRESS_BUTTON){
			setTimerCount(0); //установка таймера в 0
			displayClear();
			return;
		}
		_delay_ms(5);
	}
	displayClear();
}

//****************удаление библиотеки отпечатков*****************
void deliteFingerInLibrary(){
	setTimerCount(0); //установка таймера в 0
	displayClear();
	displayShowStr(2,0,"удалить библиотеку");
	displayShowStr(6,1, "отпечатков");
	char liblary = 1;
	char array[1];
	char result;
	while(getTimerCount() != 3){
		sprintf(array, "%d", liblary);
		displayShowStr(10,2, array);
		timerOff();
		enkoderState = getStateEncoder();
		timerOn();
		if(enkoderState == ROTATE_LEFT && liblary > 1){
			setTimerCount(0); //установка таймера в 0
			--liblary;
		}else if(enkoderState == ROTATE_RIGHT && liblary < 3){
			setTimerCount(0); //установка таймера в 0
			++liblary;
		}else if(enkoderState == PRESS_BUTTON){
			setTimerCount(0); //установка таймера в 0
			result = deleteFingerInLibrary(liblary);
			if(result)
				displayShowStr(0,3,"библиотека удалена");
			else
				displayShowStr(0,3,"ошибка удаления   ");
			_delay_ms(1000);
			displayClear();
			return;
		}else if(enkoderState == LONG_PRESS_BUTTON){
			setTimerCount(0); //установка таймера в 0
			displayClear();
			return;
		}
		_delay_ms(5);
	}
	displayClear();
}

//***********настройка таймера двери*************
void setTimerDoorMenu(){
	setTimerCount(0); //установка таймера в 0
	displayClear();
	displayShowStr(3,0,"таймер двери");
	unsigned int doorTimer = readValueTimerCloseDoor() * 5;//цена деления 5 сек
	char min[3];
	char sec[3];
	unsigned char secondNum;
	unsigned char minutNum;
	while(getTimerCount() != 3){
		minutNum = doorTimer/60; //минуты
		secondNum = doorTimer%60; //секунды
		sprintf(sec, "%d", secondNum);
		sprintf(min, "%d", minutNum);
		if(minutNum < 10){ 
			displayShowStr(4,1, " ");
			displayShowStr(5,1, min);
		}else
			 displayShowStr(4,1, min);
		displayShowStr(6,1, "мин");
		if(secondNum < 10) {
			displayShowStr(11,1, sec);
			displayShowStr(10,1, " ");
		}else
			 displayShowStr(10,1, sec);
		displayShowStr(12,1, "сек");
		timerOff();
		enkoderState = getStateEncoder();
		timerOn();
		if(enkoderState == ROTATE_LEFT && doorTimer > 5){
			setTimerCount(0); //установка таймера в 0
			doorTimer -= 5;
		}else if(enkoderState == ROTATE_RIGHT && doorTimer < 900){
			setTimerCount(0); //установка таймера в 0
			doorTimer += 5;
		}else if(enkoderState == PRESS_BUTTON){
			setTimerCount(0); //установка таймера в 0
			writeValueTimerCloseDoor(doorTimer);
			displayClear();
			return;
		} else if(enkoderState == LONG_PRESS_BUTTON){
		setTimerCount(0); //установка таймера в 0
		displayClear();
		return;
		}
		_delay_ms(5);
	}
	displayClear();
}