// Meatest_M602.cpp : 定义 DLL 对应的Meatest_M602类的各个成员函数。

#include "stdafx.h"
#include "Meatest_M602.h"
#include <iostream>
#include <Windows.h>
#include <string>
using namespace std;

//this is the member fucntions of the Meatest_M602。
#define SUCCESS 					0L
#define WRONGCOMMAND				1L
#define FAILINOPERATION				2L
#define INITIALFALED				3L
#define DEVICEOFFLINE				4L

#define EXECUTE_OK					"EXECUTE OK"
#define EMULATION_OK				"Emulation OK" 
#define EMULATION_FAILED			"Emulation Failed"
#define WRONG_COMMAND				"Meatest_M602 Error: Can't Find Command"
#define ERROR_FAIL_OPERATION		"Meatest_M602 Error: Fail"
#define INITIAL_FAILED				"Meatest_M602 Error: Initial Failed"
#define DEVICE_OFFLINE				"Meatest_M602 Error: Device Offline"

//Begin Operation{
#define Meatest_M602_RESET						"Reset"
#define Meatest_M602_SETRESISTANCEVALUE			"SetResistanceValue"
#define Meatest_M602_GETRESISTANCEVALUE			"GetResistanceValue"
#define Meatest_M602_SETMODULEADDRESS			"SetModuleAdress"
//} End Operation{
static ViSession defaultRM;
static ViUInt32 retCount;
static ViUInt32 writeCount;
static ViStatus vistatus;
static unsigned char buffer[200];
static ViSession instr;
static ViSession instr_re;

class Meatest_M602 : public iDevice {
public:
	Meatest_M602(char * devAdress, bool beSimulate = false){
		bSimulate = beSimulate;
		session = NULL;
		status = SUCCESS;
		Separator = NULL;
		modueAddr = 0x0002;
		memset(errorMsg, 0, sizeof(errorMsg));
		memset(seperateStr, 0, sizeof(seperateStr));
		memset(Result, 0, sizeof(Result));
		memset(address, 0, sizeof(address));
		//the inital of the device
		strcpy_s(address, devAdress);
		//if this the simulate module,it just return the result.
		if (beSimulate){
			status = SUCCESS;
			strcpy_s(errorMsg, EMULATION_OK);
			strcpy_s(Result, EMULATION_OK);
			return;
		}
		//use the functions of the VISA to initial the device 
		vistatus = viOpenDefaultRM(&defaultRM);
		if (VI_SUCCESS > vistatus){
			status = INITIALFALED;
			strcpy_s(errorMsg, "Can't open a session to the VISA Resource Manager");
			return;
		}
		vistatus = viOpen(defaultRM, address, VI_NULL, VI_NULL, &instr);
		if (VI_SUCCESS > vistatus){
			status = INITIALFALED;
			strcpy_s(errorMsg, "Cannot open a session to the 8432_PORT2");
		}
		status = SUCCESS;
		strcpy_s(errorMsg, EXECUTE_OK);
		return;
	};

	~Meatest_M602(void){
		if (bSimulate){
			status = SUCCESS;
			strcpy_s(errorMsg, EMULATION_OK);
			strcpy_s(Result, EMULATION_OK);
		}
		vistatus = viClose(instr);
	};

	//explain and execute the command
	virtual char * ExecuteCmd(char * commandLine){
		if (1024 <= strlen(commandLine)){
			status = WRONGCOMMAND;
			strcpy_s(errorMsg, ERROR_FAIL_OPERATION);
			memset(Result, 0, sizeof(Result));
			return Result;
		}
		memset(Result, 0, sizeof(Result));
		bool bFindSeperate = SeperateCommand(commandLine);
		if (bFindSeperate) cmdLine = Seperator + 1;

	};

	//return the status of last execute
	virtual long GetLastErrorCode() {
		return status;
	}

	//update the error message, and return error message
	virtual char * GetLastErrorMsg() {
		return errorMsg;
	}

	//free the device 
	virtual void FreeDevice() {
		delete this;
	}

protected:
	bool SeperateCommand(char * commandLine);

	void Reset();
	void SetModuleAdress(char * commandLine);
	void GetResistanceValue();
	void SetResistanceValue(char * commandLine);


	bool bSimulate;			//用于指示使用哪种模式
	void * session;
	long status;
	char errorMsg[1024];
	char seperateStr[1024];
	char * Separator;
	char address[100];
	unsigned char modueAddr;
	char Result[Meatest_M602_MAXLENGTH];//return the result


};

//extern "C" _declspec(dllexport) Meatest_M602 * createDevice(char * devAdress, bool beSimulate = false);

