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
		//会话句柄不用使用了
		//session = NULL;
		status = SUCCESS;
		Seperator = NULL;
		moduleAddr = 0x0002;//这个地址代表的是什么？
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
		if (!strcmp(seperateStr, Meatest_M602_SETMODULEADDRESS)) SetModuleAddress(cmdLine);
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
void Meatest_M602::GetResistanceValue(){
	viClear(instr);
	//一些符号的ASCII：','0x2c
	unsigned char cmdTag1 = 0x2c;		//用于表示控制指令的开始
	unsigned char cmdTag2 = 0x2c;		//用于表示控制指令的开始
	unsigned char setValue = 0x41;		//use to represent set value
	unsigned char endCode = 0x0d;       //end the command
	unsigned char query = 0x3f;			//问号的acsii码代表询问电阻值
	unsigned char sendData[] = { moduleAddr,cmdTag1, cmdTag2, setValue, query, endCode };
	vistatus = viWrite(instr, (ViBuf)sendData, (ViUInt32)6, &writeCount);//发送查询指令
	if (VI_SUCCESS > vistatus){
		status = vistatus;
		strcpy_s(errorMsg, "Error writing to the device");
		return;
	}
	//下面获取读取出来的数据，都是字符串的格式。
	char readData[10];
	vistatus = viRead(instr, (ViBuf)readData, (ViUInt32)10, &retCount);//read status
	if (VI_SUCCESS > vistatus){
		status = vistatus;
		strcpy_s(errorMsg, "Error reading to the device");
		return;
	}

	status = SUCCESS;
	strcpy_s(errorMsg, EXECUTE_OK);
	//找到<cr>的位置
	char * locationOfCR = strchr(readData, 0x0d);
	*locationOfCR = '\0';
	strcpy_s(Result, readData + 2);//复制表示电阻值的字符串到结果数组里面
	return;
	}
void Meatest_M602::SetResistanceValue(char * commandLine){
	//一些符号的ASCII：','0x2c
	unsigned char cmdTag1 = 0x2c;		//用于表示控制指令的开始
	unsigned char cmdTag2 = 0x2c;		//用于表示控制指令的开始
	unsigned char setValue = 0x41;		//use to represent set value
	unsigned char endCode = 0x0d;       //end the command
	//获取指令中所要设置的值的长度
	//int lengthOfvommandLine = strlen(commandLine);
	//需要发送的字符数组
	char sendData[FRAME_LENGTH]= { moduleAddr,cmdTag1, cmdTag2, setValue ,'\0'};
	//连接字符串
	strcat_s(sendData, commandLine);
	//加上<cr>作为指令的结束
	strcat_s(sendData, "\n");
	//获取要写入的字符串的长度
	int lengthOfsendData = strlen(sendData);

	vistatus = viWrite(instr, (ViBuf)sendData, (ViInt32)lengthOfsendData, &writeCount);
	if (vistatus < VI_SUCCESS){
		status = vistatus;
		strcpy_s(errorMsg, "Error writing to the device");
		return;
	}
	status = SUCCESS;
	strcpy_s(errorMsg, EXECUTE_OK);
	strcpy_s(Result, "succeed writing to the device");
	return;
}
void Meatest_M602::SetOutput(char * commandLine){

}
void Meatest_M602::Reset(){
	//一些符号的ASCII：','0x2c
	unsigned char cmdTag1 = 0x2c;		//用于表示控制指令的开始
	unsigned char cmdTag2 = 0x2c;		//用于表示控制指令的开始
	unsigned char setValue = 0x46;		//use to represent set module
	unsigned char module = 0x31;		//模式1
	unsigned char endCode = 0x0d;       //end the command
	//获取指令中所要设置的值的长度
	//int lengthOfvommandLine = strlen(commandLine);
	//需要发送的字符数组
	char sendData[FRAME_LENGTH] = {moduleAddr,cmdTag1, cmdTag2, setValue, module,endCode,};
	vistatus = viWrite(instr, (ViBuf)sendData, (ViInt32)6, &writeCount);
	if (vistatus < VI_SUCCESS){
		status = vistatus;
		strcpy_s(errorMsg, "Error writing to the device");
		return;
	}
	status = SUCCESS;
	strcpy_s(errorMsg, EXECUTE_OK);
	strcpy_s(Result, "succeed writing to the device");
	SetResistanceValue("100");
	return;
}
//这个函数的具体功能不了解，是复制修改而来的。
void Meatest_M602::SetModuleAddress(char * commandLine){
	moduleAddr = 0x02;
	int addr = atoi(commandLine);
	if ((addr == 0) || (addr > 16)){
		status = WRONGCOMMAND;
		strcpy_s(errorMsg, WRONG_COMMAND);
		return;
	}
	else{
		moduleAddr = addr;
	}
	status = SUCCESS;
	strcpy_s(errorMsg, EXECUTE_OK);
	return;
}
Meatest_M602* createDevice(char * devAdress, bool beSimulate){
	return new Meatest_M602(devAdress, beSimulate);
}
