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
	
	if(SerialDevice.IsReady())
	{
		
		cout<<"tty open and configed."<<endl;
	}
	else
	{
		cout<<"tty is not ready."<<endl;
	}
}

void SendData2SerialPort(SerialPort& SerialDevice, std::vector<char>& data)
{	
	std::vector<char>::iterator iter;
	if(SerialDevice.IsReady())
	{
		std::cout<<"Sent data: ";
		for(iter=data.begin();iter!=data.end();iter++) 
		{
			std::cout<<*iter;
		}
		std::cout<<"\n";

		SerialDevice.SendData(data);
	}	
}

void ActionTest()
{
	using namespace std;
	
	string name = "/dev/ttyS0";
	char sendData[] = "reboot";
	BaudrateOption baud = b115200;
	DataBitsOption data = eight;
	ParityOption parity = none;
	StopBitsOption stopBits = one;
	
	SerialPort SerialDevice(name, baud, data, parity, stopBits);
	
	vector<char> buffer(sendData,sendData + sizeof(sendData));
	if(SerialDevice.IsReady())
	{
		SerialDevice.SendData(buffer);
		cout<< "send data success!"<<endl;
	}
			
}

int main()
{
	DefaultConfigTest();
	
	ActionTest();
	
	return 0;
}
