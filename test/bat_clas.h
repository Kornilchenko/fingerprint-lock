#ifndef BAT_CLAS_H_
#define BAT_CLAS_H_

class Battery
{
private :
	char m_bat;
	
public:
	Battery()
	{
		m_bat = 5;
	}
	char getBat(){return m_bat;}
};

#endif