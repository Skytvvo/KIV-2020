
#include <iostream>
#include <ctime>

extern "C"
{

	unsigned int  toPow(unsigned int a, unsigned int b)
	{

		return pow(b, a);
	}
	int  countSin(double value)
	{
		return sin(value);
	}
	int  random(int a)
	{
		if (a < 0)
			a = -a;
		if (a == 0)
			return 0;
		srand(time(NULL));
		int k = -a + (rand() % (a * 2));
		return k;
	}
	unsigned int  strLength(char* str)
	{
		if (str == nullptr)
			return 0;
		unsigned int len = 0;
		for (int i = 0; i < 256; i++)
			if (str[i] == '\0')
			{
				len = i;
				break;
			}
		return len;
	}
	int  outnum(unsigned int value)
	{
		std::cout << value;
		return 0;
	}
	int  outstr(char* ptr)
	{
		setlocale(0,"");
		if (ptr == nullptr)
		{
			std::cout << std::endl;
		}
		for (int i = 0; ptr[i] != '\0'; i++)
			std::cout << ptr[i];
		return 0;
	}
	int  outnumline(unsigned int value)
	{
		
		std::cout << value << std::endl;
		return 0;
	}
	int  outstrline(char* ptr)
	{
		setlocale(0, "");
		
		for (int i = 0; ptr[i] != '\0'; i++)
			std::cout << ptr[i];
		std::cout << std::endl;
		return 0;
	}
	int  system_pause()
	{
		system("pause");
		return 0;
	}
}