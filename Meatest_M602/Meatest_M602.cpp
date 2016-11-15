// Meatest_M602.cpp : 定义 DLL 对应的Meatest_M602类的各个成员函数。

#include "stdafx.h"
#include "Meatest_M602.h"
#include <iostream>
#include <Windows.h>
#include <string>
using namespace std;

//this is the member fucntions of the Meatest_M602。
Meatest_M602::Meatest_M602(char * devAdress, bool beSimulate = false){
		bSimulate = beSimulate;
		session = NULL;
		status = SUCCESS;
		Seperator = NULL;
		modueAddr = 0x0002;//这个地址代表的是什么？
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
	}

Meatest_M602::~Meatest_M602(void){
		if (bSimulate){
			status = SUCCESS;
			strcpy_s(errorMsg, EMULATION_OK);
			strcpy_s(Result, EMULATION_OK);
		}
		vistatus = viClose(instr);
	}

	//explain and execute the command
char * Meatest_M602::ExecuteCmd(char * commandLine){
		if (1024 <= strlen(commandLine)){
			status = WRONGCOMMAND;
			strcpy_s(errorMsg, ERROR_FAIL_OPERATION);
			memset(Result, 0, sizeof(Result));
			return Result;
		}
		memset(Result, 0, sizeof(Result));
		char * cmdLine;
		bool bFindSeperate = SeperateCommand(commandLine);
		if (bFindSeperate) cmdLine = Seperator + 1;
		else cmdLine = NULL;
		//根据函数名称调用不同的函数
		if (!strcmp(seperateStr, Meatest_M602_SETMODULEADDRESS)) SetModuleAdress(cmdLine);
		else if (!strcmp(seperateStr, Meatest_M602_SETRESISTANCEVALUE)) SetResistanceValue(cmdLine);
		else if (!strcmp(seperateStr, Meatest_M602_GETRESISTANCEVALUE)) GetResistanceValue();
		else if (!strcmp(seperateStr, Meatest_M602_RESET)) Reset();
		else{
			status = WRONGCOMMAND;
			strcpy_s(errorMsg, WRONG_COMMAND);
		}
		return Result;
	}

bool Meatest_M602::SeperateCommand(char * commandLine){
		memset(seperateStr, 0, sizeof(seperateStr));
		Seperator = strchr(commandLine, ',');
		if (Seperator == NULL){
			strcpy_s(seperateStr, commandLine);
			return false;
		}
		else{
			//复制截取出来的字符串
			strncpy_s(seperateStr, commandLine, Seperator - commandLine);
			return true;
		}
	}

	void Reset();
	void SetModuleAdress(char * commandLine);
	void GetResistanceValue();
	void SetResistanceValue(char * commandLine);
