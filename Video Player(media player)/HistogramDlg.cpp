// HistogramDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "HistogramDlg.h"
#include "afxdialogex.h"

#include "IppImage\IppDib.h"
#include "IppImage\IppImage.h"
#include "IppImage\IppConvert.h"
#include "IppImage\IppEnhance.h"

// CHistogramDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CHistogramDlg, CDialogEx)

CHistogramDlg::CHistogramDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HISTOGRAM, pParent)
{
	memset(m_Histogram, 0, sizeof(int) * 256);
}

CHistogramDlg::~CHistogramDlg()
{
}

void CHistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHistogramDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CHistogramDlg �޽��� ó�����Դϴ�.

void CHistogramDlg::SetImage(IppDib* pDib)
{
	if (pDib != NULL && pDib->GetBitCount() == 8)
	{
		IppByteImage img;
		IppDibToImage(*pDib, img);

		// ����ȭ�� ������׷��� ���Ѵ�.
		float histo[256] = { 0.f, };
		IppHistogram(img, histo);

		// ����ȭ�� ������׷����� �ִ밪�� ���Ѵ�.
		float max_histo = histo[0];
		for (int i = 1; i < 256; i++)
			if (histo[i] > max_histo) max_histo = histo[i];

		// m_Histogram �迭�� �ִ밪�� 100�� �ǵ��� ��ü �迭�� ���� �����Ѵ�.
		for (int i = 0; i < 256; i++)
		{
			m_Histogram[i] = static_cast<int>(histo[i] * 100 / max_histo);
		}
	}
	else
	{
		memset(m_Histogram, 0, sizeof(int) * 256);
	}
}


void CHistogramDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CGdiObject* pOldPen = dc.SelectStockObject(DC_PEN);

	// ������׷� �ڽ�
	dc.SetDCPenColor(RGB(128, 128, 128));
	dc.MoveTo(20, 20);
	dc.LineTo(20, 120);
	dc.LineTo(275, 120);
	dc.LineTo(275, 20);

	// �� �׷��̽����Ͽ� �ش��ϴ� ������׷� ���
	dc.SetDCPenColor(RGB(0, 0, 0));
	for (int i = 0; i < 256; i++)
	{
		dc.MoveTo(20 + i, 120);
		dc.LineTo(20 + i, 120 - m_Histogram[i]);
	}

	// �׷��̽����� ���� ���
	for (int i = 0; i < 256; i++)
	{
		dc.SetDCPenColor(RGB(i, i, i));
		dc.MoveTo(20 + i, 130);
		dc.LineTo(20 + i, 145);
	}

	dc.SelectObject(pOldPen);
}
