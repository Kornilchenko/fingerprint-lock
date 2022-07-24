#include "ScanFinder_R300.h"

unsigned char R300_adress [6]={0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF};
	unsigned char answerBuffer[28]; //переменная массив для приема данных с модуля
	unsigned char statusString[21]; //статус операции
	
	#define verify_password			0x13 //проверка пароля
	#define set_password			0x12 //установить пароль
	#define set_address				0x15 //установить адрес
	#define read_system_paramet		0x0F //читать системные параметры
	#define search_finger			0x01 //поиск пальца на датчике
	#define save_image_buffer		0x02 //сохранение отпечатка в буфер
	#define search_in_library		0x04 //поиск отпечатка в библиотеке отпечатков
	#define shablon_is_buferov		0x05 //создать шаблон отпечатка из буферов
	#define store_shablon			0x06 //сохранение шаблона в библиотеке отпечатков
	#define load_out_library		0x07 //загрузка шаблона из указаного места библиотеки в буфер
	#define delite_library			0x0C //Удаление шаблона из флэш памяти модуля
	
	//-----------Дефайны буферов и библиотек---------------
	#define buffer1_00			    0x01
	#define buffer1_01			    0x02
	#define buffer2_00			    0x03
	#define buffer2_01			    0x04
	
	//---------Библиотеки--------------
	//#define library1			    0x01 
	//#define library2			    0x65 
	//#define library3			    0xС9 
	

//-----------------инициализация-----------
void initFingerScaner() 
{
	//обнуление массива данных пришедших с модуля
	for (unsigned char i=0; i< 28; ++i) //обнуляем массив
	answerBuffer[i] = 0x00;
}

//-------------- статус выполнения на дисплей--------------------
void writeStatus( unsigned char answer)
{
	switch(answer)
	{
		case 0x00: displayShowString("Операция успешна"); break; 
		case 0x01: displayShowString("Ошибка пакета"); break;
		case 0x02: displayShowString("Палец не обнаружен"); break;
		case 0x03: displayShowString("Невозможно собрать"); break;
		case 0x04: displayShowString("Ошибка выполнения"); break;
		case 0x09: displayShowString("нет совпадений"); break;
	}
}


//------------------ Передача данных сканеру (принимает команду, номер буфера, код ячеек памяти char memory)
void sendComand(unsigned char command, char buffer, unsigned int lib, unsigned char lenging) 
{
	int sum = 0x01;
	for(unsigned char n=0; n < 6; ++n)
		USART_Transmit(R300_adress[n]);
	USART_Transmit(0x01);// индетификатор(передаем команду 01, данные 02, пакет подтверждения 07, конец передачи дааных 08)
	USART_Transmit(0x00);// 1 часть длинны последующего пакета 
	switch (command)
	{
		case 0x1D:
		case 0x01:
		case 0x05:
		case 0x0D:
		case 0x03:	USART_Transmit(0x03); //количество последующих пакетов для передачи
					USART_Transmit(command);
					sum +=0x03 + command;
					USART_Transmit(sum/256);
					USART_Transmit(sum%256);
					break;
		case 0x02:	USART_Transmit(0x04); //количество последующих пакетов для передачи
					USART_Transmit(command);
					USART_Transmit(buffer);
					sum +=0x04 + command + buffer;
					USART_Transmit(sum/256);
					USART_Transmit(sum%256);
					break;
		case 0x06:
		case 0x07:	USART_Transmit(0x06); //количество последующих пакетов для передачи
					USART_Transmit(command);
					USART_Transmit(buffer);
					if(lib <= 0xff){
						USART_Transmit(0x00);
						USART_Transmit(lib); //номер ячейи библиотеки
					} else {
						USART_Transmit(0x01);
						USART_Transmit(lib - 0x100); //номер ячейи библиотеки
					}
					
					sum +=0x06 + command + buffer + lib; 
					USART_Transmit(sum/256);
					USART_Transmit(sum%256);
					break;
		case 0x0C:	USART_Transmit(0x07);
					USART_Transmit(command);
					USART_Transmit(0x00);
					USART_Transmit(lib);
					USART_Transmit(0x00);
					USART_Transmit(lenging);
					sum +=0x07 + command + lib + lenging;
					USART_Transmit(sum/256);
					USART_Transmit(sum%256);
					break;
		case 0x04:	USART_Transmit(0x08); 
					USART_Transmit(command);
					USART_Transmit(buffer);
					USART_Transmit(0x00); 
					USART_Transmit(0x01); //начало ячейки для поиска отпечатка
					USART_Transmit(0x00);
					USART_Transmit(0x96); //количество ячеек для поиска (150. 200 это С8)
					sum +=0x08 + command + buffer + 0x01 + 0x96;
					USART_Transmit(sum/256);
					USART_Transmit(sum%256);
					break;
	}
}

//-------вычисление контрольных сумм ----------
char control_sum(unsigned char *data)
{
	int sum =0;
	unsigned char length = data[7]+data[8];
	length +=7;
	for (unsigned char i=6; i< length; ++i)
		sum += data[i];
	if(sum == ((data[length]*256)+data[length+1]))
	return 1; //если суммы совпадают
	else return 0;
}

//-------поиск пальца на датчике--------------
unsigned char searchFingerInSensor(){
	sendComand(search_finger, 0, 0, 0); //посылаем запрос на обработку приложенного пальца
	USART_reading_in_array (answerBuffer, 12); //читаем в массив входящие данные
	return answerBuffer[9];
}

//-----поиск отпечатка в библиотеке-----------
unsigned char searchFingerInLibrary(){
	sendComand(search_in_library, buffer1_00, 0, 0); //посылаем запрос на сравнение отпечатка с 1 буфера и библиотеки отпечатков
	USART_reading_in_array (answerBuffer, 16); //читаем в массив входящие данные
	return answerBuffer[9];
}

//-------удаление отпечатков из библиотеки ------------
char deleteFingerInLibrary(char numLibrary){
	unsigned char library_begin;
	if (numLibrary == 1)
		library_begin = 0x01; //01
	else if(numLibrary == 2)
		library_begin = 0x65; //101
	else if(numLibrary == 3)
		library_begin = 0xC7; //199 
	sendComand(delite_library, 00, library_begin, 100); //удаление отпечатков
	USART_reading_in_array (answerBuffer, 12); //читаем в массив входящие данные
	if(!answerBuffer[9] && control_sum(answerBuffer))
		return 1;
	else
		return 0;
}

void addFingerInLibrary(){
	
}

void changePassword(){
	
}

unsigned char saveImageInBuffer(unsigned char buffer){
	sendComand(save_image_buffer, buffer, 0, 0); //посылаем запрос на запись отпечатка в буфер
	USART_reading_in_array (answerBuffer, 12); //читаем в массив входящие данные заданной длины
	return answerBuffer[9];
}

//---------поиск свободной ячейки памяти для запоминания отпечатка------
unsigned int searchForFreeMemoryLocation(char numLibrary){
	unsigned int library_begin; //начальный номер ячейки памяти для хранения отпечатка
	unsigned int library_end; //конечный номер ячейки памяти для хранения отпечатка
	if (numLibrary == 1){
		library_begin = 0x01; //01
		library_end = 0x64; //100
	}
	else if(numLibrary == 2){
		library_begin = 0x65; //101
		library_end = 0xC8; //200
	}
	else if(numLibrary == 3){
		library_begin = 0xC9; //201
		library_end = 0x12B; //299
	}
	 /*выгружаем ячейки памяти до тех пор пока не встретим не нулевой код подтверждения
		 это будет значить что ячейка пуста можно в нее загружать отпечаток*/
	do{
		++library_begin;
		if(library_begin >= library_end) return 0; //переполнение библиотеки
		sendComand(load_out_library, buffer1_00, library_begin, 0); //загрузка шаблона в указаное место, вернет 0 если ячейка свободна
		USART_reading_in_array (answerBuffer, 12); //читаем в массив входящие данные
	} while (!answerBuffer[9]);  
	return library_begin;
}

//----------поиск отпечатка полный цикл----------------
char searchFingerR300()
{
	unsigned char answer;
	answer = searchFingerInSensor(); //посылаем запрос на обработку приложенного пальца
	if(!answer && control_sum(answerBuffer)){
		answer = saveImageInBuffer(buffer1_00); //посылаем запрос на запись отпечатка в 1 буфер
		if(!answer && control_sum(answerBuffer)){
			answer = searchFingerInLibrary(); //посылаем запрос на сравнение отпечатка с 1 буфера и библиотеки отпечатков
			if(!answer){
				//writeStatus(answer);
				return 1; //отпечаток найден
			}else{
				//writeStatus(answer);
				return 0; //отпечаток не найден
			}
		}else{
			 //writeStatus(answer);
			 return 0; // ошибка записи в буфер 
		}
	}else{
		//writeStatus(answer);
		return 0; //пальца на датчике нет
	}
}
			
//-----------добавление отпечатка в библиотеку-----------
char addingFingerR300(char numLibrary){
	unsigned char answer; //результат запроса
	char buffer = 0x01; //промежуточный буфер присваеваем номер 1
	char count = 0; //счетчик попыток считывания пальца
	
/*получаем номер свободной ячейки библиотеки*/
	 unsigned int library  = searchForFreeMemoryLocation(numLibrary);
	 if (library == 0) return 0; //переполнение библиотеки
	
/* поиск пальца и запись в буфер 1-4 пока не дойдем до 4 буфера или код подтверждения будет не 00*/
	do{
		answer = searchFingerInSensor();
		if(!answer && control_sum(answerBuffer)){ //если ок записываем в буфер 1-4
			answer = saveImageInBuffer(buffer); //посылаем запрос на запись отпечатка в 1-4 буфер
			if(!answer)
				++buffer;
		}
		++count;
		if (count > 21) //количество попыток считать палец 20
			return 0;	
	} while (!answer && buffer < 5); 
	
//создание шаблона и сохранение в библиотеку
	if(!answer && control_sum(answerBuffer)){
		sendComand(shablon_is_buferov, 0, 0, 0); //создаем шаблон из буферов
		USART_reading_in_array (answerBuffer, 12); //читаем в массив входящие данные
		if(!answerBuffer[9] && control_sum(answerBuffer)){
			sendComand(store_shablon, buffer1_01, library, 0); //сохранение шаблона в ячейку библиотеки памяти 
			USART_reading_in_array (answerBuffer, 12); //читаем в массив входящие данные
			if(!answerBuffer[9]) 
				return 1;
		}
	}
}