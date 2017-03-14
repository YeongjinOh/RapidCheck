/******************************************************************************
 *
 * IppDib.cpp
 *
 * Copyright (c) 2015~<current> by Sun-Kyoo Hwang <sunkyoo.ippbook@gmail.com>
 *
 * This source code is included in the book titled "Image Processing 
 * Programming By Visual C++ (2nd Edition)"
 *
 *****************************************************************************/

#include "stdafx.h"
#include "IppDib.h"

#define DIB_HEADER_MARKER ((WORD) ('M' << 8) | 'B')

IppDib::IppDib()
	: m_nWidth(0), m_nHeight(0), m_nBitCount(0), m_nDibSize(0), m_pDib(NULL)
{
}

IppDib::IppDib(const IppDib& dib)
	: m_nWidth(dib.m_nWidth), m_nHeight(dib.m_nHeight), m_nBitCount(dib.m_nBitCount), m_nDibSize(dib.m_nDibSize), m_pDib(NULL)
{
	if (dib.m_pDib != NULL)
	{
		m_pDib = new BYTE[m_nDibSize];
		memcpy(m_pDib, dib.m_pDib, m_nDibSize);
	}
}

IppDib::~IppDib()
{
	if (m_pDib)
		delete[] m_pDib;
}

BOOL IppDib::CreateGrayBitmap(LONG nWidth, LONG nHeight)
{
	if (m_pDib)
		DestroyBitmap();

	m_nWidth    = nWidth;
	m_nHeight   = nHeight;
	m_nBitCount = 8;

	DWORD dwWidthStep = (m_nWidth * m_nBitCount / 8 + 3) & ~3;
	DWORD dwSizeImage = (m_nHeight * dwWidthStep);
	m_nDibSize = sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD) * GetPaletteNums()) + dwSizeImage;

	m_pDib = new BYTE[m_nDibSize];
	if (m_pDib == NULL)
		return FALSE;

	// BITMAPINFOHEADER ����ü ����
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;
	lpbmi->biSize          = sizeof(BITMAPINFOHEADER);
	lpbmi->biWidth         = m_nWidth;
	lpbmi->biHeight        = m_nHeight;
	lpbmi->biPlanes        = 1;
	lpbmi->biBitCount      = m_nBitCount;
	lpbmi->biCompression   = BI_RGB;
	lpbmi->biSizeImage     = dwSizeImage;
	lpbmi->biXPelsPerMeter = 0;
	lpbmi->biYPelsPerMeter = 0;
	lpbmi->biClrUsed       = 0;
	lpbmi->biClrImportant  = 0;

	// �׷��̽����� �ȷ�Ʈ ����
	RGBQUAD* pPal = (RGBQUAD*)((BYTE*)m_pDib + sizeof(BITMAPINFOHEADER));
	for (int i = 0; i < 256; i++)
	{
		pPal->rgbBlue     = (BYTE)i;
		pPal->rgbGreen    = (BYTE)i;
		pPal->rgbRed      = (BYTE)i;
		pPal->rgbReserved = 0;
		pPal++;
	}

	// �ȼ� ������ �ʱ�ȭ
	BYTE* pData = GetDIBitsAddr();
	memset(pData, 0, dwSizeImage);

	return TRUE;
}

BOOL IppDib::CreateRgbBitmap(LONG nWidth, LONG nHeight)
{
	if (m_pDib)
		DestroyBitmap();

	m_nWidth    = nWidth;
	m_nHeight   = nHeight;
	m_nBitCount = 24;

	DWORD dwWidthStep = (m_nWidth * m_nBitCount / 8 + 3) & ~3;
	DWORD dwSizeImage = (m_nHeight * dwWidthStep);
	m_nDibSize = sizeof(BITMAPINFOHEADER) + dwSizeImage;

	m_pDib = new BYTE[m_nDibSize];
	if (m_pDib == NULL)
		return FALSE;

	// BITMAPINFOHEADER ����ü ����
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;
	lpbmi->biSize          = sizeof(BITMAPINFOHEADER);
	lpbmi->biWidth         = m_nWidth;
	lpbmi->biHeight        = m_nHeight;
	lpbmi->biPlanes        = 1;
	lpbmi->biBitCount      = m_nBitCount;
	lpbmi->biCompression   = BI_RGB;
	lpbmi->biSizeImage     = dwSizeImage;
	lpbmi->biXPelsPerMeter = 0;
	lpbmi->biYPelsPerMeter = 0;
	lpbmi->biClrUsed       = 0;
	lpbmi->biClrImportant  = 0;

	// �ȼ� ������ �ʱ�ȭ
	BYTE* pData = GetDIBitsAddr();
	memset(pData, 0, dwSizeImage);

	return TRUE;
}

void IppDib::DestroyBitmap()
{
	if (m_pDib)
	{
		delete[] m_pDib;
		m_pDib = NULL;
	}

	m_nWidth    = 0;
	m_nHeight   = 0;
	m_nBitCount = 0;
	m_nDibSize  = 0;
}

BOOL IppDib::Load(const char* filename)
{
	const char* ext = strrchr(filename, '.');
	if (!_strcmpi(ext, ".bmp"))
		return LoadBMP(filename);
	else
		return FALSE;
}

BOOL IppDib::Save(const char* filename)
{
	const char* ext = strrchr(filename, '.');
	if (!_strcmpi(ext, ".bmp"))
		return SaveBMP(filename);
	else
		return FALSE;
}

BOOL IppDib::LoadBMP(const char* filename)
{
	FILE* fp = NULL;
	fopen_s(&fp, filename, "rb");
	if (!fp)
		return false;

	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;

	// BITMAPFILEHEADER �б�
	if (fread(&bmfh, sizeof(BITMAPFILEHEADER), 1, fp) != 1) {
		fclose(fp);
		return false;
	}

	// BMP �������� ��Ÿ���� "BM" ��Ŀ�� �ִ��� Ȯ��
	if (bmfh.bfType != DIB_HEADER_MARKER) {
		fclose(fp);
		return false;
	}

	// BITMAPINFOHEADER �б�
	if (fread(&bmih, sizeof(BITMAPINFOHEADER), 1, fp) != 1) {
		fclose(fp);
		return false;
	}

	m_nWidth    = bmih.biWidth;
	m_nHeight   = bmih.biHeight;
	m_nBitCount = bmih.biBitCount;

	// �ȼ� ������ ���� ���
	DWORD dwWidthStep = (DWORD)((m_nWidth * m_nBitCount / 8 + 3) & ~3);
	DWORD dwSizeImage = m_nHeight * dwWidthStep;

	if (m_nBitCount == 24)
		m_nDibSize = sizeof(BITMAPINFOHEADER) + dwSizeImage;
	else
		m_nDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << m_nBitCount) + dwSizeImage;

	// Packed-DIB ������ �޸� ���� ���� �Ҵ�
	if (m_pDib)
		DestroyBitmap();

	m_pDib = new BYTE[m_nDibSize];
	if (m_pDib == NULL)
	{
		fclose(fp);
		return FALSE;
	}

	// ���Ϸκ��� Packed-DIB ũ�⸸ŭ�� �б�
	fseek(fp, sizeof(BITMAPFILEHEADER), SEEK_SET);
	if (fread(m_pDib, sizeof(BYTE), m_nDibSize, fp) != m_nDibSize)
	{
		delete[] m_pDib;
		m_pDib = NULL;
		fclose(fp);
		return FALSE;
	}

	// ���� �ݱ�
	fclose(fp);

	return TRUE;
}

BOOL IppDib::SaveBMP(const char* filename)
{
	if (!IsValid())
		return FALSE;

	FILE* fp;
	fopen_s(&fp, filename, "wb");
	if (!fp)
		return FALSE;

	BITMAPFILEHEADER bmfh;
	bmfh.bfType      = DIB_HEADER_MARKER;
	bmfh.bfSize      = (DWORD)sizeof(BITMAPFILEHEADER) + m_nDibSize;
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfOffBits   = (DWORD)(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		+ (sizeof(RGBQUAD) * GetPaletteNums()));

	fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(GetBitmapInfoAddr(), m_nDibSize, 1, fp);

	fclose(fp);

	return TRUE;
}

void IppDib::Draw(HDC hdc, int dx, int dy)
{
	if (m_pDib == NULL)
		return;

	LPBITMAPINFO lpbi = (LPBITMAPINFO)m_pDib;
	LPVOID lpBits = (LPVOID)GetDIBitsAddr();

	::SetDIBitsToDevice(hdc,	// hdc
		dx,					// DestX
		dy,					// DestY
		m_nWidth,			// nSrcWidth
		m_nHeight,			// nSrcHeight
		0,					// SrcX
		0,					// SrcY
		0,					// nStartScan
		m_nHeight,			// nNumScans
		lpBits,				// lpBits
		lpbi,				// lpBitsInfo
		DIB_RGB_COLORS);	// wUsage
}

void IppDib::Draw(HDC hdc, int dx, int dy, int dw, int dh, DWORD dwRop)
{
	Draw(hdc, dx, dy, dw, dh, 0, 0, m_nWidth, m_nHeight, dwRop);
}

void IppDib::Draw(HDC hdc, int dx, int dy, int dw, int dh,
	int sx, int sy, int sw, int sh, DWORD dwRop)
{
	if (m_pDib == NULL)
		return;

	LPBITMAPINFO lpbi = (LPBITMAPINFO)m_pDib;
	LPSTR lpDIBBits = (LPSTR)GetDIBitsAddr();

	SetStretchBltMode(hdc, COLORONCOLOR);
	::StretchDIBits(hdc,	// hdc
		dx,					// XDest
		dy,					// YDest
		dw,					// nDestWidth
		dh,					// nDestHeight
		sx,					// XSrc
		sy,					// YSrc
		sw,					// nSrcWidth
		sh,					// nSrcHeight
		lpDIBBits,			// lpBits
		lpbi,				// lpBitsInfo
		DIB_RGB_COLORS,		// wUsage
		SRCCOPY);			// dwROP
}

BOOL IppDib::CopyToClipboard()
{
	if (!::OpenClipboard(NULL))
		return FALSE;

	// DIB ��ü�� ���� �޸� ���� ����
	DWORD dwDibSize = GetDibSize();
	HANDLE hDib = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, dwDibSize);
	if (hDib == NULL)
	{
		::CloseClipboard();
		return FALSE;
	}

	LPVOID lpDib = ::GlobalLock((HGLOBAL)hDib);
	memcpy(lpDib, GetBitmapInfoAddr(), dwDibSize);
	::GlobalUnlock(hDib);

	// Ŭ�����忡 ������ �Է�
	::EmptyClipboard();
	::SetClipboardData(CF_DIB, hDib);
	::CloseClipboard();

	return TRUE;
}

BOOL IppDib::PasteFromClipboard()
{
	// CF_DIB Ÿ���� �ƴϸ� �����Ѵ�.
	if (!::IsClipboardFormatAvailable(CF_DIB))
		return FALSE;

	// Ŭ�� ���带 ����.
	if (!::OpenClipboard(NULL))
		return FALSE;

	// Ŭ�� ���� ������ �޾ƿ´�.
	HANDLE hDib = ::GetClipboardData(CF_DIB);
	if (hDib == NULL)
	{
		::CloseClipboard();
		return FALSE;
	}

	// �޸� ���� ũ��� DIB ��ü ũ��� ����
	DWORD dwSize = (DWORD)::GlobalSize((HGLOBAL)hDib);
	LPVOID lpDib = ::GlobalLock((HGLOBAL)hDib);

	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)lpDib;
	m_nWidth    = lpbi->biWidth;
	m_nHeight   = lpbi->biHeight;
	m_nBitCount = lpbi->biBitCount;

	DWORD dwWidthStep = (DWORD)((m_nWidth * m_nBitCount / 8 + 3) & ~3);
	DWORD dwSizeImage = m_nHeight * dwWidthStep;

	if (m_nBitCount == 24)
		m_nDibSize = sizeof(BITMAPINFOHEADER) + dwSizeImage;
	else
		m_nDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << m_nBitCount) + dwSizeImage;

	// ���� ������ IppDib ��ü�� �ִٸ� �����Ѵ�.
	if (m_pDib)
		DestroyBitmap();

	m_pDib = new BYTE[m_nDibSize];
	if (m_pDib == NULL)
	{
		::GlobalUnlock(hDib);
		::CloseClipboard();
		return FALSE;
	}

	memcpy(m_pDib, lpDib, m_nDibSize);

	::GlobalUnlock(hDib);
	::CloseClipboard();

	return TRUE;
}

IppDib& IppDib::operator=(const IppDib& dib)
{
	if (this == &dib)	// ��� �˻�
		return *this;

	if (m_pDib)
		delete[] m_pDib;

	m_nWidth    = dib.m_nWidth;
	m_nHeight   = dib.m_nHeight;
	m_nBitCount = dib.m_nBitCount;
	m_nDibSize  = dib.m_nDibSize;
	m_pDib      = NULL;

	if (dib.m_pDib != NULL)
	{
		m_pDib = new BYTE[m_nDibSize];
		memcpy(m_pDib, dib.m_pDib, m_nDibSize);
	}

	return *this;
}

BYTE* IppDib::GetDIBitsAddr() const
{
	if (m_pDib == NULL)
		return NULL;

	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;
	return ((BYTE*)m_pDib + lpbmi->biSize + (sizeof(RGBQUAD) * GetPaletteNums()));
}

int IppDib::GetPaletteNums() const
{
	switch (m_nBitCount)
	{
	case 1:     return 2;
	case 4:     return 16;
	case 8:     return 256;
	default:    return 0;
	}
}
