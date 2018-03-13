/*
*	serial_port_util.h
*
*	Create on:Mar 12,2018
*	   Author:Shen
*
*/

enum ParityOption
{
	odd,
	even,
	none,
};

enum StopBitsOption
{
	one,
	two,
};

enum DataBitsOption
{
	six = CS6,
	seven = CS7,
	eight = CS8,
};
enum BaudrateOption
{
	b0 = B0,
	b50 = B50,
	b75 = B75,
	b110 = B110,
	b134 = B134,
	b150 = B150,
	b220 = B200,
	b300 = B300,
	b600 = B600,
	b1200 = B1200,
	b1800 = B1800,
	b2400 = B2400,
	b4800 = B4800,
	b9600 = B9600,
	b19200 = B19200,
	b38400 = B38400,
	b57600 = B57600,
	b115200 = B1152000,
};