#include "stdafx.h"
#include <iostream>
#include <opencv2\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#define EXTERN __declspec(dllexport)

extern "C"
{
	EXTERN cv::Mat asd()
	{
		printf("test\n");
		cv::Mat test = cv::imread("C:/test.jpg");
		if (test.empty())
			printf("empty\n");
		cv::imshow("test", test);
		return test;
	}

	EXTERN int ADD(int a, int b)
	{
		return a + b;
	}
	EXTERN void PRINT()
	{
		std::cout << "C++ code" << std::endl;
	}
}