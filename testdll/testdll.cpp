// testdll.cpp : �������̨Ӧ�ó������ڵ㡣
#pragma comment(lib,"Meatest_M602.lib")
extern "C" _declspec(dllimport) int Max(int a, int b);
extern "C" _declspec(dllimport) int Min(int a, int b);
#include "stdafx.h"
#include <iostream>

int main()
{
	using namespace std;
	int a, b;
	a = 2;
	b = 3;
	int result;
	result = Max(a, b);
	cout << "The Max of the two number is:" << result << endl;
	cout<<"The Min of the two number is:" << Min(a, b) << std::endl;
	cin >> result;
    return 0;
}

