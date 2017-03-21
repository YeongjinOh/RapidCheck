// ClassLibrary1.h

#pragma once
#include <opencv\cv.h>
#include <opencv2\opencv.hpp>
#include <opencv2\core.hpp>
#include <opencv2\highgui\highgui.hpp>

using namespace System;

namespace ClassLibrary1 {

	public ref class MyOpenCvWrapper
	{
	public:
		System::Drawing::Bitmap^ ApplyFilter(System::Drawing::Bitmap^ bitmap);
		cv::Mat ApplyFilter2(const cv::Mat test);
		// TODO: ���⿡ �� Ŭ������ ���� �޼��带 �߰��մϴ�.
	};
}
