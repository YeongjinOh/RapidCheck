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
		// TODO: 여기에 이 클래스에 대한 메서드를 추가합니다.
	};
}
