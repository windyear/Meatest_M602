// Meatest_M602.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
//下面的代码是测试dll是否能够使用的
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
	bool SeperateCommand(char * commandLine);		//分割命令

	void Reset();									//重置，设置电阻箱的模式为电阻模式，电阻值为100欧姆
	void SetModuleAddress(char * commandLine);		//设置模块的地址
	void GetResistanceValue();						//获取电阻箱的电阻值
	void SetResistanceValue(char * commandLine);	//设置电阻箱的电阻值
	void SetOutput(char * commandLine);								//设置电阻箱的开关，这个函数只能在不是提供外部电源的时候使用，暂时不实现


	bool bSimulate;			//用于指示使用哪种模式
	//void * session;			//会话句柄
	long status;			//表示状态
	char errorMsg[1024];	//错误信息
	char seperateStr[1024]; //保存截取的字符串
	char * Seperator;		//分隔符位置指示
	char address[100];		//NI串口板卡的地址
	unsigned char moduleAddr;//电阻箱模块的地址
	char Result[Meatest_M602_MAXLENGTH];//return the result


};

extern "C" _declspec(dllexport) Meatest_M602 * createDevice(char * devAdress, bool beSimulate = false);

