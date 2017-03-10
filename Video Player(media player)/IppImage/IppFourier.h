/******************************************************************************
*
* IppFourier.h
*
* Copyright (c) 2015~<current> by Sun-Kyoo Hwang <sunkyoo.ippbook@gmail.com>
*
* This source code is included in the book titled "Image Processing
* Programming By Visual C++ (2nd Edition)"
*
******************************************************************************/

#pragma once

#include "IppImage.h"

class IppFourier
{
public:
	int width;
	int height;
	IppDoubleImage real; // �Ǽ���
	IppDoubleImage imag; // �����

public:
	IppFourier();

	void SetImage(IppByteImage& img);
	void GetImage(IppByteImage& img);
	void GetSpectrumImage(IppByteImage& img);
	void GetPhaseImage(IppByteImage& img);

	// ������ Ǫ���� ��ȯ �Լ�
	void DFT(int dir);
	void DFTRC(int dir);
	void FFT(int dir);

	// ���ļ� ���������� ���͸� �Լ�
	void LowPassIdeal(int cutoff);
	void HighPassIdeal(int cutoff);
	void LowPassGaussian(int cutoff);
	void HighPassGaussian(int cutoff);
};

// ���� �Լ� ����
void DFT1d(double* re, double* im, int N, int dir);
void FFT1d(double* re, double* im, int N, int dir);
bool IsPowerOf2(int n);
