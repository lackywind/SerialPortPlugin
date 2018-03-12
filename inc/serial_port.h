/*
*	serial_port.h
*
*	Create on:Mar 12,2018
*	  Atuthor:Shen
*
*/

#include <termios.h> //tty device configuration
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <cstring>
#include <iostream>

#include "file_descriptor_owner.h"




enum ParityOption
{
        odd,
        even,
        none,
}Parity = none;
 
enum StopBitsOption
{
        one,
        two,
}StopBits = one;
 
enum DataBitsOption
{
        six = CS6,
        seven = CS7,
        eight = CS8,
}DataBits = eight;

class SerialPort
{
public:
	SerialPort(std::string device,unsigned int baudRate = B115200,DataBitsOption dataBits = DataBits,ParityOption parity = Parity,StopBitsOption stopBits = StopBits);
	virtual ~SerialPort();
	void OpenDevice(std::string);
	void Configure(unsigned int baudRate,DataBitsOption dataBits,ParityOption parity,StopBitsOption stopBits); 
	
private:
	int _fileDescriptor; // device file descriptor
	int _isConfigured; // device set configuration or not
	int _isOpen; // device is oped or not 	
};

