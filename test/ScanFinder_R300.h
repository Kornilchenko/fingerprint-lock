#ifndef SCANFINDER_R300_H_
#define SCANFINDER_R300_H_

#include "main.h"
//-------------�������������-----------
void initFingerScaner();
//-------- ������ ���������� �� �������--------------
void writeStatus(unsigned char answer);
//-------����� ������ �� �������--------------
unsigned char searchFingerInSensor();
//----�������� ���������� �� ���������� -----------
char deleteFingerInLibrary(char numLibrary);
//---------����� ��������� ������ ����----------------
char searchFingerR300();
//--------���������� ��������� � ����������-----------
char addingFingerR300(char numLibrary);


#endif /* SCANFINDER_R300_H_ */