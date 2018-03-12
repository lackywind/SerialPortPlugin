/*
*	main.cpp
*
*	Create on:Mar 12.2018
*	   Author:Shen
*
*/

#include "serial_port.h"
#include <string>

void DefaultConfigTest()
{
	using namespace std;

	string name = "/dev/ttyS0";
	
	SerialPort SerialDevice(name);

	cout<<"config test success!"<<endl;
}


int main()
{
	DefaultConfigTest();

	return 0;
}
