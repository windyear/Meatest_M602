// Meatest_M602.cpp : 定义 DLL 对应的Meatest_M602类的各个成员函数。

#include "stdafx.h"
#include "Meatest_M602.h"
#include <iostream>
#include <Windows.h>
#include <string>
using namespace std;

//this is the member fucntions of the Meatest_M602。
Meatest_M602::Meatest_M602(char * devAdress, bool beSimulate){
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
		//逗号作为分隔符
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
void Meatest_M602::GetResistanceValue(){
	//一些符号的ASCII：','0x2c
	unsigned char cmdTag1 = 0x2c;		//用于表示控制指令的开始
	unsigned char cmdTag2 = 0x2c;		//用于表示控制指令的开始
	unsigned char setValue = 0x41;		//use to represent set value
	unsigned char endCode = 0x0d;       //end the command
	unsigned char query = 0x3f;			//问号的acsii码代表询问电阻值
	unsigned char sendData[] = { cmdTag1, cmdTag2, setValue, query, endCode };

	vistatus = viWrite(instr, (ViBuf)sendData, (ViUInt32)5, &writeCount);
	if (VI_SUCCESS > vistatus){
		status = vistatus;
		strcpy_s(errorMsg, "Error writing to the device");
		return;
	}
	//下面获取读取出来的数据，都是字符串的格式。

	}
void Meatest_M602::SetResistanceValue(char * commandLine){
	//一些符号的ASCII：','0x2c
	unsigned char cmdTag1 = 0x2c;		//用于表示控制指令的开始
	unsigned char cmdTag2 = 0x2c;		//用于表示控制指令的开始
	unsigned char setValue = 0x41;		//use to represent set value
	unsigned char endCode = 0x0d;       //end the command
	

	//获取指令中所要设置的值的长度
	int lengthOfvommandLine = strlen(commandLine);
	
	unsigned char sendData[] = { cmdTag1, cmdTag2, setValue };


}
void Meatest_M602::SetOutput(char * commandLine){

}
void Meatest_M602::Reset(){

}
void Meatest_M602::SetModuleAdress(char * commandLine){

}
Meatest_M602* createDevice(char * devAdress, bool beSimulate){
	return new Meatest_M602(devAdress, beSimulate);
}
