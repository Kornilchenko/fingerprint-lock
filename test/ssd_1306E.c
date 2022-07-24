#include "ssd_1306E.h"

//--------дефайны для дисплея 
#define SSD1306_adress      0b01111000 //адрес устройства на шине
#define COM					0b10000000  //следующим байтом будет команда
#define DAT					0b11000000  //следующим байтом будет байт данных 
#define DATS				0b01000000  //все последующие байты будут данными


//	Функция передачи команды дисплею
//-------------------------------------------------------------------------
void ssd1306_send_com(char com)
{
	I2C_StartCondition();
	I2C_SendByte(SSD1306_adress);
	I2C_SendByte(COM);
	I2C_SendByte(com);
	I2C_StopCondition();
}

//	Функция передачи данных дисплею
//-------------------------------------------------------------------------
void ssd1306_send_data(char data)
{
	I2C_StartCondition();
	I2C_SendByte(SSD1306_adress);
	I2C_SendByte(DAT);
	I2C_SendByte(data);
	I2C_StopCondition();
}

//	Функция иницциализирует дисплей. 
//-------------------------------------------------------------------------
void display_128x64_Init(void)
{
	_delay_ms(500);
	I2C_StartCondition();
	I2C_SendByte(SSD1306_adress);
	I2C_SendByte(COM);
	I2C_SendByte(0xAE);   //*выключить дисплей
	
	I2C_SendByte(COM);
	I2C_SendByte(0xA8); //*установить multiplex ratio
	I2C_SendByte(COM);
	I2C_SendByte(0x3F);//0x3F*--1/64 duty (значение по умолчанию)
	
	I2C_SendByte(COM);
	I2C_SendByte(0xD3); //Установите вертикальный сдвиг(Смещение дисплея (offset))
	I2C_SendByte(COM);
	I2C_SendByte(0x00); //Установливаем вертикальный сдвиг на 00 (нет смещения)
	
	I2C_SendByte(COM);
	I2C_SendByte(0x40); //Установить начальную строку дисплея(с начала RAM)
	
	I2C_SendByte(COM);
	I2C_SendByte(0xA1); //Режим развертки по странице (по X) A1 - нормальный режим (слева/направо) A0 - обратный (справа/налево)
	I2C_SendByte(COM);
	I2C_SendByte(0xC8); //выводим данные построчно C8-сверху вниз, С0-снизу вверх, режим сканирования озу дисплея //c0
	// для изменения системы координат
	// С0 - снизу/верх (начало нижний левый угол)
	// С8 - сверху/вниз (начало верхний левый угол)
	
	I2C_SendByte(COM);
	I2C_SendByte(0xDA); //Установить конфигурацию оборудования COM-контактов
	I2C_SendByte(COM);
	I2C_SendByte(0x12); //(СБРОС), альтернативная конфигурация контактов COM
	
	I2C_SendByte(COM);
	I2C_SendByte(0x81);  //Установить контроль контрастности
	I2C_SendByte(COM);
	I2C_SendByte(0x70); //Контрастность максимум FF (0-255)
	
	I2C_SendByte(COM);
	I2C_SendByte(0xA4); //Весь дисплей включен Возобновить отображение содержимого ОЗУ (СБРОС)
	
	I2C_SendByte(COM);
	I2C_SendByte(0xA6); //Нормальный дисплей (СБРОС) нет инверсии
	
	I2C_SendByte(COM);
	I2C_SendByte(0xD9);   //настройка фаз DC/DC преобразователя
	I2C_SendByte(COM);
	I2C_SendByte(0xF1);   //0x22-VCC подается извне / 0xF1 для внутреннего
	
	I2C_SendByte(COM);
	I2C_SendByte(0xD5); //Установить коэффициент деления частоты дисплея / частоту осциллятора
	I2C_SendByte(COM);
	I2C_SendByte(0x80); //Установите частоту осциллятора, FOSC.(СБРОС), Определите коэффициент деления (D) отображаемых часов (DCLK): RESET
	
	I2C_SendByte(COM);
	I2C_SendByte(0x8D);// Управление внутренним преобразователем
	I2C_SendByte(COM);
	I2C_SendByte(0x14);//0x10 - отключить (VCC подается извне) 0x14 - запустить внутренний DC/DC
	
	I2C_SendByte(COM);
	I2C_SendByte(0xDB);   // Установка VcomH
	I2C_SendByte(COM);
	I2C_SendByte(0x40);   // влияет на яркость дисплея 0x30 (0x00-0x10-0x20-0x30-0x40-0x50-0x60-0x70)
	
	I2C_SendByte(COM);
	I2C_SendByte(0x20);   // режима автоматической адресации
	I2C_SendByte(0x00);      // 0-по горизонтали с переходом на новую страницу (строку)
	//1 - по вертикали с переходом на новую строку
	//2 - только по выбранной странице без перехода
	
	//    OLEDwrite_comm(0x22);        // размер дисплея по Y
	//   OLEDwrite_comm(0);           // от 0 страницы
	//    OLEDwrite_comm(7);           // до 7 страницы
	
	//    OLEDwrite_comm(0x21);        // размер дисплея по X
	//   OLEDwrite_comm(0);           // от 0 колонки
	//    OLEDwrite_comm(127);         // до 127 колонки
	
	I2C_SendByte(COM);
	I2C_SendByte(0xAF); //Дисплей включен в нормальном режиме

	I2C_StopCondition(); //Стоп
}

//*************** Функция установки позиции ******************************
void displaySetPosition(char x, char y)
{
	if((y >= 0) && (y <= 7)) // если в диапазоне 
	{
		ssd1306_send_com(y+0xB0); //что бы поменять станицы местами 7- 0, 6- 1 и т. д.(7-y+0xB0)
	}
	ssd1306_send_com(((x & 0xF0) >> 4) | 0x10);
	ssd1306_send_com((x & 0x0F) | 0x01);
}

//*************** Функция очистки дисплея **********************************

void displayClear(void)
{
	displaySetPosition(0,7); //выставляем на нулевую позицию
	I2C_StartCondition();
	I2C_SendByte(SSD1306_adress); //посылаем адресс устройства
	I2C_SendByte(DATS);
	for (int n = 0; n < 1024; ++n)
		I2C_SendByte(0x00); //заполняем все пространство нулями
	I2C_StopCondition(); //стоп
}

//*************** Функция очистки одной строки **********************************
void displayClearString(char numString){
	displaySetPosition(0,numString); //выставляем на строку
	I2C_StartCondition();
	I2C_SendByte(SSD1306_adress); //посылаем адресс устройства
	I2C_SendByte(DATS);
	for (char n = 0; n < 128; ++n)
	I2C_SendByte(0x00); //заполняем все пространство нулями
	I2C_StopCondition(); //стоп
}

//************	Функция установки курсора (отступ на колчества символов) *********************

void displayGotoPosition(char x, char y)
{
	if(x > 15) x = 21;
	if(y > 7) y = 7;
	displaySetPosition((x*6), y);
}

//**************** Функция вывода символа **************************************************

void displayShowChar(char chr)
{	
	if ((chr < 0x20)||((chr > 0x7E)&&(chr < 0xC0)))
			chr = 0x00; //считаем знак равным 0x00 и выведем его как пробел 
	else if ((chr >= 0x20)&&(chr <= 0x7E))
			chr -= 0x20; //если знак символа больше 0x20 (пробел) по таблице ASCII и до 0x7E(тильды)
			else chr -= 0x61; //для вывода кирилицы (работает с нашим масивом шрифта)
	
	 	for(char i=0; i<5; i++)
				 //считаем где начало (номер знака умноженный на 5 строчек в массиве вывода знака)  
	 			//ssd1306_send_data(font6x8[(chr*5)+i]); //иттерация вывода массива шрифта
				 ssd1306_send_data(EEPROM_read_word((chr*5)+i)); //вывод через EEPROM
	ssd1306_send_data(0x00); //добавляем пустой столбец
}
 
//******************* Функция вывода строки на позицию************************************************

void displayShowStr(char x, char y, char *str) // вводим координаты а затем текст
{
	displayGotoPosition(x, y); //позиционируем начало 
	char length = 0; //счетчик выведенных символов
	while(*str)
	{
		if (length >= 21) //если вывели 21 символ в строке
		{
			if(y >= 7) //проверяем не последняя ли это строка
			displayGotoPosition(0, y = 0); //если посленяя сторка переходим на нулевую строку
			else displayGotoPosition(0, ++y); //если не последняя строка переходим на следующую строку и стаем на начало 
			length = 0; //сбрасываем счетчик когда доходим до конца строки
		}
		displayShowChar(*str); //выводим символ
		str++;
		++length;
	}
}

//******************* Функция вывода строки ************************************************

void displayShowString(char *str) // вводим координаты а затем текст
{
	while(*str)
	{
		displayShowChar(*str); //выводим символ
		str++;
	}
}
//*******************************	Функция вывода своего символа ****************************

//void ssd136_symbol(char *arr, char light)
//{
	//char i = 0;
	//
	//for(i=0; i<light; i++)
	//{
		//ssd1306_send_data(*arr);
		//arr++;
	//}
//}
