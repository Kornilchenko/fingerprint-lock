#include "ScanFinder_R300.h"

unsigned char R300_adress [6]={0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF};
	unsigned char answerBuffer[28]; //���������� ������ ��� ������ ������ � ������
	unsigned char statusString[21]; //������ ��������
	
	#define verify_password			0x13 //�������� ������
	#define set_password			0x12 //���������� ������
	#define set_address				0x15 //���������� �����
	#define read_system_paramet		0x0F //������ ��������� ���������
	#define search_finger			0x01 //����� ������ �� �������
	#define save_image_buffer		0x02 //���������� ��������� � �����
	#define search_in_library		0x04 //����� ��������� � ���������� ����������
	#define shablon_is_buferov		0x05 //������� ������ ��������� �� �������
	#define store_shablon			0x06 //���������� ������� � ���������� ����������
	#define load_out_library		0x07 //�������� ������� �� ��������� ����� ���������� � �����
	#define delite_library			0x0C //�������� ������� �� ���� ������ ������
	
	//-----------������� ������� � ���������---------------
	#define buffer1_00			    0x01
	#define buffer1_01			    0x02
	#define buffer2_00			    0x03
	#define buffer2_01			    0x04
	
	//---------����������--------------
	//#define library1			    0x01 
	//#define library2			    0x65 
	//#define library3			    0x�9 
	

//-----------------�������������-----------
void initFingerScaner() 
{
	//��������� ������� ������ ��������� � ������
	for (unsigned char i=0; i< 28; ++i) //�������� ������
	answerBuffer[i] = 0x00;
}

//-------------- ������ ���������� �� �������--------------------
void writeStatus( unsigned char answer)
{
	switch(answer)
	{
		case 0x00: displayShowString("�������� �������"); break; 
		case 0x01: displayShowString("������ ������"); break;
		case 0x02: displayShowString("����� �� ���������"); break;
		case 0x03: displayShowString("���������� �������"); break;
		case 0x04: displayShowString("������ ����������"); break;
		case 0x09: displayShowString("��� ����������"); break;
	}
}


//------------------ �������� ������ ������� (��������� �������, ����� ������, ��� ����� ������ char memory)
void sendComand(unsigned char command, char buffer, unsigned int lib, unsigned char lenging) 
{
	int sum = 0x01;
	for(unsigned char n=0; n < 6; ++n)
		USART_Transmit(R300_adress[n]);
	USART_Transmit(0x01);// �������������(�������� ������� 01, ������ 02, ����� ������������� 07, ����� �������� ������ 08)
	USART_Transmit(0x00);// 1 ����� ������ ������������ ������ 
	switch (command)
	{
		case 0x1D:
		case 0x01:
		case 0x05:
		case 0x0D:
		case 0x03:	USART_Transmit(0x03); //���������� ����������� ������� ��� ��������
					USART_Transmit(command);
					sum +=0x03 + command;
					USART_Transmit(sum/256);
					USART_Transmit(sum%256);
					break;
		case 0x02:	USART_Transmit(0x04); //���������� ����������� ������� ��� ��������
					USART_Transmit(command);
					USART_Transmit(buffer);
					sum +=0x04 + command + buffer;
					USART_Transmit(sum/256);
					USART_Transmit(sum%256);
					break;
		case 0x06:
		case 0x07:	USART_Transmit(0x06); //���������� ����������� ������� ��� ��������
					USART_Transmit(command);
					USART_Transmit(buffer);
					if(lib <= 0xff){
						USART_Transmit(0x00);
						USART_Transmit(lib); //����� ����� ����������
					} else {
						USART_Transmit(0x01);
						USART_Transmit(lib - 0x100); //����� ����� ����������
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
					USART_Transmit(0x01); //������ ������ ��� ������ ���������
					USART_Transmit(0x00);
					USART_Transmit(0x96); //���������� ����� ��� ������ (150. 200 ��� �8)
					sum +=0x08 + command + buffer + 0x01 + 0x96;
					USART_Transmit(sum/256);
					USART_Transmit(sum%256);
					break;
	}
}

//-------���������� ����������� ���� ----------
char control_sum(unsigned char *data)
{
	int sum =0;
	unsigned char length = data[7]+data[8];
	length +=7;
	for (unsigned char i=6; i< length; ++i)
		sum += data[i];
	if(sum == ((data[length]*256)+data[length+1]))
	return 1; //���� ����� ���������
	else return 0;
}

//-------����� ������ �� �������--------------
unsigned char searchFingerInSensor(){
	sendComand(search_finger, 0, 0, 0); //�������� ������ �� ��������� ������������ ������
	USART_reading_in_array (answerBuffer, 12); //������ � ������ �������� ������
	return answerBuffer[9];
}

//-----����� ��������� � ����������-----------
unsigned char searchFingerInLibrary(){
	sendComand(search_in_library, buffer1_00, 0, 0); //�������� ������ �� ��������� ��������� � 1 ������ � ���������� ����������
	USART_reading_in_array (answerBuffer, 16); //������ � ������ �������� ������
	return answerBuffer[9];
}

//-------�������� ���������� �� ���������� ------------
char deleteFingerInLibrary(char numLibrary){
	unsigned char library_begin;
	if (numLibrary == 1)
		library_begin = 0x01; //01
	else if(numLibrary == 2)
		library_begin = 0x65; //101
	else if(numLibrary == 3)
		library_begin = 0xC7; //199 
	sendComand(delite_library, 00, library_begin, 100); //�������� ����������
	USART_reading_in_array (answerBuffer, 12); //������ � ������ �������� ������
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
	sendComand(save_image_buffer, buffer, 0, 0); //�������� ������ �� ������ ��������� � �����
	USART_reading_in_array (answerBuffer, 12); //������ � ������ �������� ������ �������� �����
	return answerBuffer[9];
}

//---------����� ��������� ������ ������ ��� ����������� ���������------
unsigned int searchForFreeMemoryLocation(char numLibrary){
	unsigned int library_begin; //��������� ����� ������ ������ ��� �������� ���������
	unsigned int library_end; //�������� ����� ������ ������ ��� �������� ���������
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
	 /*��������� ������ ������ �� ��� ��� ���� �� �������� �� ������� ��� �������������
		 ��� ����� ������� ��� ������ ����� ����� � ��� ��������� ���������*/
	do{
		++library_begin;
		if(library_begin >= library_end) return 0; //������������ ����������
		sendComand(load_out_library, buffer1_00, library_begin, 0); //�������� ������� � �������� �����, ������ 0 ���� ������ ��������
		USART_reading_in_array (answerBuffer, 12); //������ � ������ �������� ������
	} while (!answerBuffer[9]);  
	return library_begin;
}

//----------����� ��������� ������ ����----------------
char searchFingerR300()
{
	unsigned char answer;
	answer = searchFingerInSensor(); //�������� ������ �� ��������� ������������ ������
	if(!answer && control_sum(answerBuffer)){
		answer = saveImageInBuffer(buffer1_00); //�������� ������ �� ������ ��������� � 1 �����
		if(!answer && control_sum(answerBuffer)){
			answer = searchFingerInLibrary(); //�������� ������ �� ��������� ��������� � 1 ������ � ���������� ����������
			if(!answer){
				//writeStatus(answer);
				return 1; //��������� ������
			}else{
				//writeStatus(answer);
				return 0; //��������� �� ������
			}
		}else{
			 //writeStatus(answer);
			 return 0; // ������ ������ � ����� 
		}
	}else{
		//writeStatus(answer);
		return 0; //������ �� ������� ���
	}
}
			
//-----------���������� ��������� � ����������-----------
char addingFingerR300(char numLibrary){
	unsigned char answer; //��������� �������
	char buffer = 0x01; //������������� ����� ����������� ����� 1
	char count = 0; //������� ������� ���������� ������
	
/*�������� ����� ��������� ������ ����������*/
	 unsigned int library  = searchForFreeMemoryLocation(numLibrary);
	 if (library == 0) return 0; //������������ ����������
	
/* ����� ������ � ������ � ����� 1-4 ���� �� ������ �� 4 ������ ��� ��� ������������� ����� �� 00*/
	do{
		answer = searchFingerInSensor();
		if(!answer && control_sum(answerBuffer)){ //���� �� ���������� � ����� 1-4
			answer = saveImageInBuffer(buffer); //�������� ������ �� ������ ��������� � 1-4 �����
			if(!answer)
				++buffer;
		}
		++count;
		if (count > 21) //���������� ������� ������� ����� 20
			return 0;	
	} while (!answer && buffer < 5); 
	
//�������� ������� � ���������� � ����������
	if(!answer && control_sum(answerBuffer)){
		sendComand(shablon_is_buferov, 0, 0, 0); //������� ������ �� �������
		USART_reading_in_array (answerBuffer, 12); //������ � ������ �������� ������
		if(!answerBuffer[9] && control_sum(answerBuffer)){
			sendComand(store_shablon, buffer1_01, library, 0); //���������� ������� � ������ ���������� ������ 
			USART_reading_in_array (answerBuffer, 12); //������ � ������ �������� ������
			if(!answerBuffer[9]) 
				return 1;
		}
	}
}