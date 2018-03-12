/*
*	serial_port.cpp
*
*	Create on:Mar 12,2018
*	   Author:Shen
*
*/


#include "serial_port.h"

SerialPort::SerialPort(std::string device,unsigned int baudRate,DataBitsOption dataBits,ParityOption parity,StopBitsOption stopBits)
{
	OpenDevice(device);
	Configure(bauRate,dataBits,parity,stopBits);
}

SerialPort::~SerialPort()
{
	close(_fileDescriptor);
}

/*
* @brief open tty device given in input argument.
*
* @param device serial device name
*
*/
void SerialPort::OpenDevice(std::string device)
{
	int fileFlags = O_RDWR | O_NOCTTY | O_NDELAY;
	
	_fileDescriptor = open(device.c_str(),fileFlags);
	if(-1 == _fileDescriptor)
	{
		printf("Cant open the file.\r\n");
	}
	fcntl(_fileDescriptor, F_SETFL,0);		
	_isOpend = true;
}

/*
* @brief Configure tty device.
*
* @param baudRate device baud rate
* @param dataBits number of data bits
* @param parity parity check
* @param stopBits 
*
*/
void SerialPort::Configure(unsigned int baudRate,DataBitsOption dataBits,ParityOption parity,StopBitsOption stopBits);
{
	// structure used to tty device configuration
	termios config;
	if(!isatty(_fileDescriptor))
	{
		printf("Open file is not tty device.\r\n");
	}
		
	// get current configuration of the serial interface		
	if(tcgetattr(_fileDescriptor,&config) <0 )
	{
		printf("Can not get serial interface configuration.\r\n");
	}

	// Input flags - Turn off input processing
	// convert break to null byte, no CR to NL translation,
	// no NL to CR translation
	config.c_iflag &= ~(IGNBRK|BRKINT|ICRNL|INLCR);

	// disable software flow control
	config.c_iflag &= ~(IXON|IXOFF|IXANY);

	// output flags - trun off output processing - raw ouput 
	config.c_iflag &= ~(OPOST);
		
	// no line processing - raw input
        // echo off, echo newline off, canonical mode off,
        // extended input processing off, signal chars off
        config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
	
	// configure parity
	switch(parity)
	{   
       	case ParityOption::none:
               	config.c_cflag &= ~PARENB; // disable parity check
               	config.c_iflag &= ~(INPCK|PARMRK|ISTRIP); // disable input parity check and parity bit strip
               	break;
       	case ParityOption::odd:
               	config.c_cflag |= (PARENB|PARODD); // enable odd parity
               	config.c_iflag |= (INPCK|PARMRK|ISTRIP); // enable input parity check and parity bit strip
               	break;
       	case ParityOption::even:
               	config.c_cflag |= PARENB; // enable even parity
               	config.c_cflag &= ~PARODD;
               	config.c_iflag |= (INPCK|PARMRK|ISTRIP); // enable input parity check and parity bit strip
               	break;
       	}
		
	// configure stop bits
	switch(stopBits)
	{
	case StopBitsOption::one:
		config.c_cflag &= ~CSTOPB;
		break;
	case StopBitsOption::two:
		config.c_cflag |= CSTOPB;
		break;
	}


	// configure data bits
	config.c_cflag &= ~CSIZE;
	config.c_cflag |= static_cast<unsigned int>(data_bits);

	// If you want to change configurationn,please follow this site:
	// http://www.cmrr.umn.edu/~strupp/serial.html#3_1
		
	// driver will read input bytes program won't 'own' port		
	config.c_cflag |= (CLOCAL|CREAD);

	// set up control characters
	config.c_cc[VMIN] = 1; // one input byte is enough to return from read()
	config.c_cc[VTIME] = 0;// inter character timer off

	// communication speed		
	if(cfsetispeed(&config,baudRate) < 0 || cfsetospeed(&config,baudRate) < 0)
	{
		printf("Set baud error.\r\n");
	}

	// apply the configuration ( flush buffers and apply )
       	if(tcsetattr(_fileDescriptor, TCSAFLUSH, &config) < 0)
       	{
               	 printf("Cannot apply new configuration.\r\n");
       	}
       	_isConfigured = true;
}

