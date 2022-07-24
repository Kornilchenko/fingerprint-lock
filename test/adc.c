#include "adc.h"

unsigned int adc_value;

//*********вернем значение АЦП ***********
unsigned int get_ADC_value(){
	return adc_value;
}
char high_adc=0,low_adc=0;

ISR(ADC_vect)
{
	low_adc = ADCL;
	high_adc = ADCH;//Верхняя часть регистра ADC должна быть считана последней, иначе не продолжится преобразование
	adc_value = high_adc * 256 + low_adc;
}

//*********инициализация АЦП***********
void ADC_Init(void)
{
	ADCSRA |= (1<<ADEN) // Разрешение использования АЦП
	|(1<<ADSC)//Запуск преобразования
	|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)//Делитель 128 = 64 кГц
	|(1<<ADIE)//Разрешение прерывания от АЦП
	|(1<<ADATE);//Автоматическое циклическое измерение показаниц если в регистре SFIOR ADTS2-ADTS0 =0 для атмега16 
 ADMUX |= (1<<REFS1)|(1<<REFS0); //Внутренний Источник ОН 2,56в, вход ADC2
}