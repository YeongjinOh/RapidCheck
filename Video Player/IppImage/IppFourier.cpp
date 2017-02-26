/******************************************************************************
*
* IppFourier.cpp
*
* Copyright (c) 2015~<current> by Sun-Kyoo Hwang <sunkyoo.ippbook@gmail.com>
*
* This source code is included in the book titled "Image Processing
* Programming By Visual C++ (2nd Edition)"
*
*****************************************************************************/

#include "StdAfx.h"
#include "IppFourier.h"

#include <math.h>
#include <algorithm>

const double PI = 3.14159265358979323846;

IppFourier::IppFourier()
	: width(0), height(0)
{
}

void IppFourier::SetImage(IppByteImage& img)
{
	width = img.GetWidth();
	height = img.GetHeight();

	real.Convert(img);               // �Ǽ��ο� ������ �׷��̽����� �� ����
	imag.CreateImage(width, height); // ����δ� 0���� �ʱ�ȭ
}

void IppFourier::GetImage(IppByteImage& img)
{
	if (!real.IsValid())
		return;

	// �Ǽ��θ� �̿��Ͽ� ��� ������ ����
	img.CreateImage(width, height);

	double* pRe = real.GetPixels();
	BYTE* ptr = img.GetPixels();

	int size = real.GetSize();
	for (int i = 0; i < size; i++)
		ptr[i] = static_cast<BYTE>(limit(pRe[i] + 0.5));
}

void IppFourier::GetSpectrumImage(IppByteImage& img)
{
	register int i, j;

	img.CreateImage(width, height);
	BYTE** ptr = img.GetPixels2D();

	double** pRe = real.GetPixels2D();
	double** pIm = imag.GetPixels2D();

	// ����Ʈ�� ���� ������ �ӽ� ����
	IppDoubleImage spectrum(width, height);
	double** pSpec = spectrum.GetPixels2D();

	//-------------------------------------------------------------------------
	// ���Ҽ� ���� ���밪�� �α� ��ȯ�Ͽ� ����. 
	// ����ȭ�� ��� �̹��� ������ ���Ͽ� ����Ʈ���� �ִ밪 ���.
	//-------------------------------------------------------------------------

	double mag, max_value = 0;
	for (j = 0; j < height; j++)
	for (i = 0; i < width; i++)
	{
		mag = sqrt(pRe[j][i] * pRe[j][i] + pIm[j][i] * pIm[j][i]);
		pSpec[j][i] = log(mag + 1);

		if (pSpec[j][i] > max_value)
			max_value = pSpec[j][i];
	}

	//-------------------------------------------------------------------------
	// ��� ���� ����. Ǫ���� ��ȯ ����� SHIFT �ϰ�, 
	// ��� ������ �ִ밪�� 255�� �ǵ��� �׷��̽����� ���� ����ȭ.
	//-------------------------------------------------------------------------

	int x, y;
	for (j = 0; j < height; j++)
	for (i = 0; i < width; i++)
	{
		x = i + width / 2;
		y = j + height / 2;

		if (x >= width) x -= width;
		if (y >= height) y -= height;

		ptr[j][i] = static_cast<BYTE>(limit(pSpec[y][x] * 255 / max_value));
	}
}

void IppFourier::GetPhaseImage(IppByteImage& img)
{
	register int i, j;

	img.CreateImage(width, height);
	BYTE** ptr = img.GetPixels2D();

	double** pRe = real.GetPixels2D();
	double** pIm = imag.GetPixels2D();

	// ������ ������ �ӽ� ����
	IppDoubleImage phase(width, height);
	double** pPhase = phase.GetPixels2D();

	//-------------------------------------------------------------------------
	// ���Ҽ� ���� ������ ����. atan2 �Լ��� ��ȯ ���� -PI ~ PI �̴�.
	//-------------------------------------------------------------------------

	for (j = 0; j < height; j++)
	for (i = 0; i < width; i++)
	{
		pPhase[j][i] = atan2(pIm[j][i], pRe[j][i]); // ����� ����
	}

	//-------------------------------------------------------------------------
	// ��� ���� ����. Ǫ���� ��ȯ ����� SHIFT �ϰ�, 
	// ��� ������ �ִ밪�� 255�� �ǵ��� �׷��̽����� ���� ����ȭ.
	//-------------------------------------------------------------------------

	int x, y;
	for (j = 0; j < height; j++)
	for (i = 0; i < width; i++)
	{
		x = i + width / 2;
		y = j + height / 2;

		if (x >= width) x -= width;
		if (y >= height) y -= height;

		ptr[j][i] = static_cast<BYTE>(limit(pPhase[y][x] * 128 / PI + 128));
	}
}

void IppFourier::DFT(int dir)
{
	if (!real.IsValid())
		return;

	//-------------------------------------------------------------------------
	// real, imag ���纻 ����
	//-------------------------------------------------------------------------

	IppDoubleImage real_cpy = real;
	IppDoubleImage imag_cpy = imag;

	double** pRe = real.GetPixels2D();
	double** pIm = imag.GetPixels2D();

	double** pReCpy = real_cpy.GetPixels2D();
	double** pImCpy = imag_cpy.GetPixels2D();

	//-------------------------------------------------------------------------
	// �̻� Ǫ���� ��ȯ (�Ǵ� ����ȯ)
	//-------------------------------------------------------------------------

	register int i, j, x, y;
	double sum_re, sum_im, temp;

	for (j = 0; j < height; j++)
	for (i = 0; i < width; i++)
	{
		sum_re = sum_im = 0;

		for (y = 0; y < height; y++)
		for (x = 0; x < width; x++)
		{
			temp = 2 * dir * PI * (static_cast<double>(i) * x / width + static_cast<double>(j) * y / height);
			sum_re += (pReCpy[y][x] * cos(temp) - pImCpy[y][x] * sin(temp));
			sum_im += (pReCpy[y][x] * sin(temp) + pImCpy[y][x] * sin(temp));
		}

		pRe[j][i] = sum_re;
		pIm[j][i] = sum_im;

		if (dir == -1) // ����ȯ�� ���
		{
			pRe[j][i] /= (width * height);
			pIm[j][i] /= (width * height);
		}
	}
}

void IppFourier::DFTRC(int dir)
{
	if (!real.IsValid())
		return;

	//-------------------------------------------------------------------------
	// ��(Row) ���� �̻� Ǫ���� ��ȯ (�Ǵ� ����ȯ)
	//-------------------------------------------------------------------------

	IppDoubleImage real_tmp, imag_tmp;

	real_tmp.CreateImage(width, 1);
	imag_tmp.CreateImage(width, 1);

	double* re = real_tmp.GetPixels();
	double* im = imag_tmp.GetPixels();

	double** pRe = real.GetPixels2D();
	double** pIm = imag.GetPixels2D();

	register int i, j;

	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			re[i] = pRe[j][i];
			im[i] = pIm[j][i];
		}

		DFT1d(re, im, width, dir);

		for (i = 0; i < width; i++)
		{
			pRe[j][i] = re[i];
			pIm[j][i] = im[i];
		}
	}

	//-------------------------------------------------------------------------
	// ��(Column) ���� �̻� Ǫ���� ��ȯ (�Ǵ� ����ȯ)
	//-------------------------------------------------------------------------

	real_tmp.CreateImage(height, 1);
	imag_tmp.CreateImage(height, 1);

	re = real_tmp.GetPixels();
	im = imag_tmp.GetPixels();

	memset(re, 0, sizeof(double) * height);
	memset(im, 0, sizeof(double) * height);

	for (i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			re[j] = pRe[j][i];
			im[j] = pIm[j][i];
		}

		DFT1d(re, im, height, dir);

		for (j = 0; j < height; j++)
		{
			pRe[j][i] = re[j];
			pIm[j][i] = im[j];
		}
	}
}

void IppFourier::FFT(int dir)
{
	if (!real.IsValid())
		return;

	if (!IsPowerOf2(width) || !IsPowerOf2(height))
		return;

	//-------------------------------------------------------------------------
	// ��(Row) ���� �̻� Ǫ���� ��ȯ (�Ǵ� ����ȯ)
	//-------------------------------------------------------------------------

	IppDoubleImage real_tmp, imag_tmp;

	real_tmp.CreateImage(width, 1);
	imag_tmp.CreateImage(width, 1);

	double* re = real_tmp.GetPixels();
	double* im = imag_tmp.GetPixels();

	double** pRe = real.GetPixels2D();
	double** pIm = imag.GetPixels2D();

	register int i, j;

	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			re[i] = pRe[j][i];
			im[i] = pIm[j][i];
		}

		FFT1d(re, im, width, dir);

		for (i = 0; i < width; i++)
		{
			pRe[j][i] = re[i];
			pIm[j][i] = im[i];
		}
	}

	//-------------------------------------------------------------------------
	// ��(Column) ���� �̻� Ǫ���� ��ȯ (�Ǵ� ����ȯ)
	//-------------------------------------------------------------------------

	real_tmp.CreateImage(height, 1);
	imag_tmp.CreateImage(height, 1);

	re = real_tmp.GetPixels();
	im = imag_tmp.GetPixels();

	memset(re, 0, sizeof(double) * height);
	memset(im, 0, sizeof(double) * height);

	for (i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			re[j] = pRe[j][i];
			im[j] = pIm[j][i];
		}

		FFT1d(re, im, height, dir);

		for (j = 0; j < height; j++)
		{
			pRe[j][i] = re[j];
			pIm[j][i] = im[j];
		}
	}
}

void IppFourier::LowPassIdeal(int cutoff)
{
	register int i, j, x, y;

	int cx = width / 2;
	int cy = height / 2;

	double** pRe = real.GetPixels2D();
	double** pIm = imag.GetPixels2D();

	for (j = 0; j < height; j++)
	for (i = 0; i < width; i++)
	{
		x = i + cx;
		y = j + cy;

		if (x >= width)  x -= width;
		if (y >= height) y -= height;

		if ((x - cx)*(x - cx) + (y - cy)*(y - cy) > (cutoff * cutoff))
			pRe[j][i] = pIm[j][i] = 0.;
	}
}

void IppFourier::HighPassIdeal(int cutoff)
{
	register int i, j, x, y;

	int cx = width / 2;
	int cy = height / 2;

	double** pRe = real.GetPixels2D();
	double** pIm = imag.GetPixels2D();
	
	for (j = 0; j < height; j++)
	for (i = 0; i < width; i++)
	{
		x = i + cx;
		y = j + cy;

		if (x >= width) x -= width;
		if (y >= height) y -= height;

		if ((x - cx)*(x - cx) + (y - cy)*(y - cy) < (cutoff * cutoff))
			pRe[j][i] = pIm[j][i] = 0;
	}
}

void IppFourier::LowPassGaussian(int cutoff)
{
	register int i, j, x, y;
	double dist2, hval;

	int cx = width / 2;
	int cy = height / 2;

	double** pRe = real.GetPixels2D();
	double** pIm = imag.GetPixels2D();

	for (j = 0; j < height; j++)
	for (i = 0; i < width; i++)
	{
		x = i + cx;
		y = j + cy;

		if (x >= width) x -= width;
		if (y >= height) y -= height;

		dist2 = static_cast<double>((x - cx)*(x - cx) + (y - cy)*(y - cy));

		hval = exp(-dist2 / (2 * cutoff * cutoff));

		pRe[j][i] *= hval;
		pIm[j][i] *= hval;
	}
}

void IppFourier::HighPassGaussian(int cutoff)
{
	register int i, j, x, y;
	double dist2, hval;

	int cx = width / 2;
	int cy = height / 2;

	double** pRe = real.GetPixels2D();
	double** pIm = imag.GetPixels2D();

	for (j = 0; j < height; j++)
	for (i = 0; i < width; i++)
	{
		x = i + cx;
		y = j + cy;

		if (x >= width) x -= width;
		if (y >= height) y -= height;

		dist2 = static_cast<double>((x - cx)*(x - cx) + (y - cy)*(y - cy));

		hval = 1.0 - exp(-dist2 / (2 * cutoff * cutoff));

		pRe[j][i] *= hval;
		pIm[j][i] *= hval;
	}
}

//-------------------------------------------------------------------------
// ���� �Լ� ����
//-------------------------------------------------------------------------

void DFT1d(double* re, double* im, int N, int dir)
{
	double* tr = new double[N];
	double* ti = new double[N];

	memcpy(tr, re, sizeof(double) * N);
	memcpy(ti, im, sizeof(double) * N);

	register int i, x;
	double sum_re, sum_im, temp;
	for (i = 0; i < N; i++)
	{
		sum_re = sum_im = 0;

		for (x = 0; x < N; x++)
		{
			temp = 2 * dir * PI * i * x / N;

			sum_re += (tr[x] * cos(temp) + ti[x] * sin(temp));
			sum_im += (ti[x] * cos(temp) - tr[x] * sin(temp));
		}

		re[i] = sum_re;
		im[i] = sum_im;
	}

	if (dir == -1) // IDFT
	{
		for (i = 0; i < N; i++)
		{
			re[i] /= static_cast<double>(N);
			im[i] /= static_cast<double>(N);
		}
	}

	delete[] tr;
	delete[] ti;
}

void FFT1d(double* re, double* im, int N, int dir)
{
	register int i, j, k;

	//-------------------------------------------------------------------------
	// �Է� ������ ���� �ٲٱ�
	//-------------------------------------------------------------------------

	int n2 = N >> 1;
	int nb = 0;

	while (N != (1 << nb))
		nb++;

	for (i = 0, j = 0; i < N - 1; i++)
	{
		if (i < j)
		{
			std::swap(re[i], re[j]);
			std::swap(im[i], im[j]);
		}

		k = n2;

		while (k <= j)
		{
			j -= k;
			k >>= 1;
		}

		j += k;
	}

	//-------------------------------------------------------------------------
	// �����ö���(Butterfly) �˰���
	//-------------------------------------------------------------------------

	int i1, l, l1, l2;
	double c1, c2, t1, t2, u1, u2, z;

	c1 = -1.0;
	c2 = 0.0;
	l2 = 1;

	for (l = 0; l < nb; l++)
	{
		l1 = l2;
		l2 <<= 1;
		u1 = 1.0;
		u2 = 0.0;

		for (j = 0; j < l1; j++)
		{
			for (i = j; i < N; i += l2)
			{
				i1 = i + l1;
				t1 = u1 * re[i1] - u2 * im[i1];
				t2 = u1 * im[i1] + u2 * re[i1];
				re[i1] = re[i] - t1;
				im[i1] = im[i] - t2;
				re[i] += t1;
				im[i] += t2;
			}

			z = u1 * c1 - u2 * c2;
			u2 = u1 * c2 + u2 * c1;
			u1 = z;
		}

		c2 = sqrt((1.0 - c1) / 2.0);

		if (dir == 1) // Forward
			c2 = -c2;

		c1 = sqrt((1.0 + c1) / 2.0);
	}

	if (dir == -1) // IDFT
	{
		for (i = 0; i < N; i++)
		{
			re[i] /= static_cast<double>(N);
			im[i] /= static_cast<double>(N);
		}
	}
}

bool IsPowerOf2(int n)
{
	int ref = 1;

	while (ref < n)
		ref <<= 1;

	if (ref == n)
		return true;
	else
		return false;
}
