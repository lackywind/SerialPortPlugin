/*
*	serial_port.h
*
*	Create on:Mar 12,2018
*	  Atuthor:Shen
*
*/

#ifndef SERIALPORT_H_
#define SERIALPORT_H_


#include <termios.h> //tty device configuration
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <sys/ioctl.h>
#include <unistd.h>

#include "file_descriptor_owner.h"
#include "serial_port_util.h"

class SerialPort
{
public:
	SerialPort(std::string device,unsigned int baudRate = B115200,DataBitsOption dataBits = eight,ParityOption parity = none,StopBitsOption stopBits = one);
	virtual ~SerialPort();
	void OpenDevice(std::string device);
	void Configure(unsigned int baudRate,DataBitsOption dataBits,ParityOption parity,StopBitsOption stopBits); 
	void SendData(const std::vector<char>& buffer);
	void ReadData();
	int IsDataReady();
	
	bool IsOpen(){return _isOpen;}
	bool IsReady(){return _isConfigured && _isOpen;}
	bool IsConfigured(){return _isConfigured;}
	
private:
	const int _dataBufferSize = 60;
	std::vector<char> _receivedDataBuffer;
	int _fileDescriptor; // device file descriptor
	bool _isConfigured; // device set configuration or not
	bool _isOpen; // device is oped or not 	
};

#endif
