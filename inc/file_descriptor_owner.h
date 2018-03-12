/*
*	file_descriptor_owner.h
*
*	Create on:Mar 12,2018
*	   Author:Shen
*
*/


class FileDescriptorOwner
{
public:
	virtual void ProcessData()=0;
	virtual int GetFileDescriptor()=0;
	virtual ~FileDescriptorOwner();	
};

