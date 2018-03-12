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
	virtual void processData()=0;
	virtual int getFileDescriptor()=0;
	virtual ~FileDescriptorOwner();	
};

