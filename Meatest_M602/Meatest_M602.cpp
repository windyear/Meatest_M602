// Meatest_M602.cpp : ���� DLL Ӧ�ó���ĵ���������
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

