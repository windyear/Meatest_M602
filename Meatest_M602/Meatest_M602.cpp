// Meatest_M602.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
extern "C" _declspec(dllexport) int Max(int a, int b);
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
}

