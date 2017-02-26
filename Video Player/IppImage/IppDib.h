/******************************************************************************
 *
 * IppDib.h
 *
 * Copyright (c) 2015~<current> by Sun-Kyoo Hwang <sunkyoo.ippbook@gmail.com>
 *
 * This source code is included in the book titled "Image Processing 
 * Programming By Visual C++ (2nd Edition)"
 *
 *****************************************************************************/

#pragma once

#include <windows.h>

class IppDib
{
public:
	// �����ڿ� �Ҹ���
	IppDib();
	IppDib(const IppDib& dib);
	~IppDib();

	// ��Ʈ�� ������ �Ҹ�
	BOOL CreateGrayBitmap(LONG nWidth, LONG nHeight);
	BOOL CreateRgbBitmap(LONG nWidth, LONG nHeight);
	void DestroyBitmap();

	// ���� �����
	BOOL Load(const char* filename);
	BOOL Save(const char* filename);

	// ��Ʈ�� ȭ�� ���
	void Draw(HDC hdc, int dx = 0, int dy = 0);
	void Draw(HDC hdc, int dx, int dy, int dw, int dh, DWORD dwRop = SRCCOPY);
	void Draw(HDC hdc, int dx, int dy, int dw, int dh, int sx, int sy, int sw,
		int sh, DWORD dwRop = SRCCOPY);

	// Windows ȯ�濡�� Ŭ������� �����ϱ� �� �ٿ��ֱ�
	BOOL CopyToClipboard();
	BOOL PasteFromClipboard();

	// ���� ������ ������
	IppDib& operator=(const IppDib& dib);
	
	// ��Ʈ�� ���� ��ȯ
	LONG        GetWidth()    const { return m_nWidth; }
	LONG        GetHeight()	  const { return m_nHeight; }
	WORD        GetBitCount() const { return m_nBitCount; }
	DWORD       GetDibSize()  const { return m_nDibSize;  }
	LPBITMAPINFO GetBitmapInfoAddr() const { return (LPBITMAPINFO)m_pDib; }
	BYTE*       GetDIBitsAddr() const;
	int         GetPaletteNums() const;
	BOOL        IsValid() const { return (m_pDib != NULL); }

private:
	// BMP ���� �����
	BOOL        LoadBMP(const char* filename);
	BOOL        SaveBMP(const char* filename);

private:
	LONG    m_nWidth;      // ��Ʈ�� ���� ũ�� (�ȼ� ����)
	LONG    m_nHeight;     // ��Ʈ�� ���� ũ�� (�ȼ� ����)
	WORD    m_nBitCount;   // �ȼ� �� ��Ʈ ��
	DWORD   m_nDibSize;    // DIB ��ü ũ�� (BITMAPINFOHEADER + ���� ���̺� + �ȼ� ������)
	BYTE*   m_pDib;        // DIB ���� �ּ� (BITMAPINFOHEADER ���� �ּ�)
};

