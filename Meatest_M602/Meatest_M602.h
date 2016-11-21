// Meatest_M602.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
//����Ĵ����ǲ���dll�Ƿ��ܹ�ʹ�õ�
/*extern "C" _declspec(dllexport) int Max(int a, int b);
extern "C" _declspec(dllexport) int Min(int a, int b);
int Max(int a, int b)
{
if (a >= b)return a;
else
return b;
}
int Min(int a, int b)
{
if (a >= b)return b;
else
return a;
}*/
//this is the head file of the Meatest_M602
#pragma once

#include "idevice.h"
#include "visa.h"
#include "visatype.h"

#define Meatest_M602_MAXLENGTH		10240

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
#define Meatest_M602_SETMODULEADDRESS			"SetModuleAddress"
//} End Operation{
#define FRAME_LENGTH 100
static ViSession defaultRM;
static ViUInt32 retCount;
static ViUInt32 writeCount;
static ViStatus vistatus;
static unsigned char buffer[200];
static ViSession instr;
static ViSession instr_re;

class Meatest_M602 : public iDevice {
public:
	Meatest_M602(char * devAdress, bool beSimulate = false);

	~Meatest_M602(void);

	//explain and execute the command
	virtual char * ExecuteCmd(char * commandLine);

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
	bool SeperateCommand(char * commandLine);		//�ָ�����

	void Reset();									//���ã����õ������ģʽΪ����ģʽ������ֵΪ100ŷķ
	void SetModuleAddress(char * commandLine);		//����ģ��ĵ�ַ
	void GetResistanceValue();						//��ȡ������ĵ���ֵ
	void SetResistanceValue(char * commandLine);	//���õ�����ĵ���ֵ
	void SetOutput(char * commandLine);								//���õ�����Ŀ��أ��������ֻ���ڲ����ṩ�ⲿ��Դ��ʱ��ʹ�ã���ʱ��ʵ��


	bool bSimulate;			//����ָʾʹ������ģʽ
	//void * session;			//�Ự���
	long status;			//��ʾ״̬
	char errorMsg[1024];	//������Ϣ
	char seperateStr[1024]; //�����ȡ���ַ���
	char * Seperator;		//�ָ���λ��ָʾ
	char address[100];		//NI���ڰ忨�ĵ�ַ
	unsigned char moduleAddr;//������ģ��ĵ�ַ
	char Result[Meatest_M602_MAXLENGTH];//return the result


};

extern "C" _declspec(dllexport) Meatest_M602 * createDevice(char * devAdress, bool beSimulate = false);

