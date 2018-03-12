/*
*	serial_port_util.h
*
*	Create on:Mar 12,2018
*	   Author:Shen
*
*/
enum class ParityOption
{
	odd,
	even,
	none,
};

enum class StopBitsOption
{
	one,
	two,
};

enum class DataBitsOption
{
	six = CS6,
	seven = CS7,
	eigth = CS8,
};
