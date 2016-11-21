// Meatest_M602.cpp : ���� DLL ��Ӧ��Meatest_M602��ĸ�����Ա������

#include "stdafx.h"
#include "Meatest_M602.h"
#include <iostream>
#include <Windows.h>
#include <string>
using namespace std;

//this is the member fucntions of the Meatest_M602��
Meatest_M602::Meatest_M602(char * devAdress, bool beSimulate){
		bSimulate = beSimulate;
		//�Ự�������ʹ����
		//session = NULL;
		status = SUCCESS;
		Seperator = NULL;
		moduleAddr = 0x0002;//�����ַ�������ʲô��
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
		//���ݺ������Ƶ��ò�ͬ�ĺ���
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
		//������Ϊ�ָ���
		Seperator = strchr(commandLine, ',');
		if (Seperator == NULL){
			strcpy_s(seperateStr, commandLine);
			return false;
		}
		else{
			//���ƽ�ȡ�������ַ���
			strncpy_s(seperateStr, commandLine, Seperator - commandLine);
			return true;
		}
	}
void Meatest_M602::GetResistanceValue(){
	viClear(instr);
	//һЩ���ŵ�ASCII��','0x2c
	unsigned char cmdTag1 = 0x2c;		//���ڱ�ʾ����ָ��Ŀ�ʼ
	unsigned char cmdTag2 = 0x2c;		//���ڱ�ʾ����ָ��Ŀ�ʼ
	unsigned char setValue = 0x41;		//use to represent set value
	unsigned char endCode = 0x0d;       //end the command
	unsigned char query = 0x3f;			//�ʺŵ�acsii�����ѯ�ʵ���ֵ
	unsigned char sendData[] = { moduleAddr,cmdTag1, cmdTag2, setValue, query, endCode };
	vistatus = viWrite(instr, (ViBuf)sendData, (ViUInt32)6, &writeCount);//���Ͳ�ѯָ��
	if (VI_SUCCESS > vistatus){
		status = vistatus;
		strcpy_s(errorMsg, "Error writing to the device");
		return;
	}
	//�����ȡ��ȡ���������ݣ������ַ����ĸ�ʽ��
	char readData[10];
	vistatus = viRead(instr, (ViBuf)readData, (ViUInt32)10, &retCount);//read status
	if (VI_SUCCESS > vistatus){
		status = vistatus;
		strcpy_s(errorMsg, "Error reading to the device");
		return;
	}

	status = SUCCESS;
	strcpy_s(errorMsg, EXECUTE_OK);
	//�ҵ�<cr>��λ��
	char * locationOfCR = strchr(readData, 0x0d);
	*locationOfCR = '\0';
	strcpy_s(Result, readData + 2);//���Ʊ�ʾ����ֵ���ַ����������������
	return;
	}
void Meatest_M602::SetResistanceValue(char * commandLine){
	//һЩ���ŵ�ASCII��','0x2c
	unsigned char cmdTag1 = 0x2c;		//���ڱ�ʾ����ָ��Ŀ�ʼ
	unsigned char cmdTag2 = 0x2c;		//���ڱ�ʾ����ָ��Ŀ�ʼ
	unsigned char setValue = 0x41;		//use to represent set value
	unsigned char endCode = 0x0d;       //end the command
	//��ȡָ������Ҫ���õ�ֵ�ĳ���
	//int lengthOfvommandLine = strlen(commandLine);
	//��Ҫ���͵��ַ�����
	char sendData[FRAME_LENGTH]= { moduleAddr,cmdTag1, cmdTag2, setValue ,'\0'};
	//�����ַ���
	strcat_s(sendData, commandLine);
	//����<cr>��Ϊָ��Ľ���
	strcat_s(sendData, "\n");
	//��ȡҪд����ַ����ĳ���
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
	//һЩ���ŵ�ASCII��','0x2c
	unsigned char cmdTag1 = 0x2c;		//���ڱ�ʾ����ָ��Ŀ�ʼ
	unsigned char cmdTag2 = 0x2c;		//���ڱ�ʾ����ָ��Ŀ�ʼ
	unsigned char setValue = 0x46;		//use to represent set module
	unsigned char module = 0x31;		//ģʽ1
	unsigned char endCode = 0x0d;       //end the command
	//��ȡָ������Ҫ���õ�ֵ�ĳ���
	//int lengthOfvommandLine = strlen(commandLine);
	//��Ҫ���͵��ַ�����
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
//��������ľ��幦�ܲ��˽⣬�Ǹ����޸Ķ����ġ�
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
