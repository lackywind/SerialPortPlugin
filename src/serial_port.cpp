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
	_isConfigured = false;
	_isOpen = false;
	OpenDevice(device);
	Configure(baudRate,dataBits,parity,stopBits);
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
		std::cout<<"Cant open the file."<<std::endl;
	}
	fcntl(_fileDescriptor, F_SETFL,0);		
	_isOpen = true;
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
void SerialPort::Configure(unsigned int baudRate,DataBitsOption dataBits,ParityOption parity,StopBitsOption stopBits)
{
	// structure used to tty device configuration
	termios config;
	if(!isatty(_fileDescriptor))
	{
		std::cout<<"Open file is not tty device"<<std::endl;
	}
		
	// get current configuration of the serial interface		
	if(tcgetattr(_fileDescriptor,&config) <0 )
	{
		std::cout<<"Can not get serial interface configuration."<<std::endl;
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
       	case none:
            config.c_cflag &= ~PARENB; // disable parity check
            config.c_iflag &= ~(INPCK|PARMRK|ISTRIP); // disable input parity check and parity bit strip
            break;
       	case odd:
            config.c_cflag |= (PARENB|PARODD); // enable odd parity
            config.c_iflag |= (INPCK|PARMRK|ISTRIP); // enable input parity check and parity bit strip
            break;
       	case even:
            config.c_cflag |= PARENB; // enable even parity
            config.c_cflag &= ~PARODD;
            config.c_iflag |= (INPCK|PARMRK|ISTRIP); // enable input parity check and parity bit strip
            break;
       	}
		
	// configure stop bits
	switch(stopBits)
	{
	case one:
		config.c_cflag &= ~CSTOPB;
		break;
	case two:
		config.c_cflag |= CSTOPB;
		break;
	}


	// configure data bits
	config.c_cflag &= ~CSIZE;
	config.c_cflag |= static_cast<unsigned int>(dataBits);

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
		std::cout<<"Set baudrate error."<<std::endl;
	}

	// apply the configuration ( flush buffers and apply )
       	if(tcsetattr(_fileDescriptor, TCSAFLUSH, &config) < 0)
       	{
               	 std::cout<<"Cannot apply new configuration."<<std::endl;
       	}
       	_isConfigured = true;
}

/*
 * @brief Checks if there is data ready to read.
 * @return size of data to read in bytes
 */
int SerialPort::IsDataReady()
{
	// data ready to read
	int bytes = 0;

	ioctl(_fileDescriptor, FIONREAD, &bytes); // check number of bytes ready to read

	return bytes;
}

/*
 * @brief Sends data through serial port
 *
 * @param buffer holds data to send
 * @throws serial_port_exception
 */
void SerialPort::SendData(const std::vector<char>& buffer)
{	
	std::vector<char>::const_iterator iter;
	int length = buffer.size();
	char outBuffer[length];

	int i = 0;
	for(iter=buffer.begin();iter!=buffer.end();iter++)
	{
		outBuffer[i++] = *iter;
	}

	// write data to file
	int writtenBytes = write(_fileDescriptor, outBuffer, length);

	if (writtenBytes < 0)
		std::cout<<"Error when sending data."<<std::endl;
	else if (writtenBytes < length)
		std::cout<<"Less elements written than expected."<<std::endl;
}

/**
 * @brief Read data from serial port
 */
void SerialPort::ReadData()
{
	//std::unique_lock<std::mutex> lock{_fd_mutex};

	char readBuffer[_dataBufferSize];
	std::memset(readBuffer, 0, _dataBufferSize);

	int readBytes = read(_fileDescriptor, readBuffer, _dataBufferSize);

	if(readBytes < 0)
		std::cout<<"Error when reading data from serial port"<<std::endl;

	_receivedDataBuffer.clear();
	for(int i = 0; i<readBytes; i++)
	{
		_receivedDataBuffer.push_back(readBuffer[i]);
	}
}


