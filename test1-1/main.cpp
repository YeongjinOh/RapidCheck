#include "stdafx.h"
#include <iostream>
#define EXTERN __declspec(dllexport)

extern "C"
{
	EXTERN int ADD(int a, int b)
	{
		return a + b;
	}
	EXTERN void PRINT()
	{
		std::cout << "C++ code" << std::endl;
	}
}