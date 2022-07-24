#ifndef SCANFINDER_R300_H_
#define SCANFINDER_R300_H_

#include "main.h"
//-------------инициализация-----------
void initFingerScaner();
//-------- статус выполнения на дисплей--------------
void writeStatus(unsigned char answer);
//-------поиск пальца на датчике--------------
unsigned char searchFingerInSensor();
//----удаление отпечатков из библиотеки -----------
char deleteFingerInLibrary(char numLibrary);
//---------поиск отпечатка полный цикл----------------
char searchFingerR300();
//--------добавление отпечатка в библиотеку-----------
char addingFingerR300(char numLibrary);


#endif /* SCANFINDER_R300_H_ */