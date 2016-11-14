#pragma once



class iDevice
{
public:
	//explain and execute the command 
	virtual char* ExecuteCmd(char* commandLine) = 0;
	//return the status of last execute
	virtual long  GetLastErrorCode() = 0;
	//update the error message, and return error message
	virtual char* GetLastErrorMsg() = 0;
	//Free Device
    virtual void FreeDevice() = 0;

};

