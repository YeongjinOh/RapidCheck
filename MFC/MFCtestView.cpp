
// MFCtestView.cpp : CMFCtestView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "MFCtest.h"
#endif

#include "MFCtestDoc.h"
#include "MFCtestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCtestView

IMPLEMENT_DYNCREATE(CMFCtestView, CView)

BEGIN_MESSAGE_MAP(CMFCtestView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMFCtestView ����/�Ҹ�

CMFCtestView::CMFCtestView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.


}

CMFCtestView::~CMFCtestView()
{
}

BOOL CMFCtestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMFCtestView �׸���

//void CMFCtestView::OnDraw(CDC* pDC)
void CMFCtestView::OnDraw(CDC* /*pDC*/)
{
	/*
	CMFCtestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int i, dan;
    CString str;

    for (dan = 2; dan <= 9; dan++)
    {
        for (i = 1; i <= 2; i++)
        {
            str.Format(_T("%dx%d=%d"), dan, i, dan * i);
            pDC->TextOut((dan - 2) * 70 + 20, 20 * i, str);
        }
    }
	str.Format(_T("test %d"), 31);
	pDC->TextOut(100, 180, str); //x,y ��ǥ, ����� ����
	*/
	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CMFCtestView �μ�

BOOL CMFCtestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMFCtestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMFCtestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CMFCtestView ����

#ifdef _DEBUG
void CMFCtestView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCtestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCtestDoc* CMFCtestView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCtestDoc)));
	return (CMFCtestDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCtestView �޽��� ó����

#define DIB_HEADER_MARKER ((WORD) ('M' << 8) | 'B')
void CMFCtestView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	FILE* fp = NULL;
	fopen_s(&fp, "screen.bmp", "rb");
	if (!fp)
		return;

	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;

	// BITMAPFILEHEADER �б�
	if (fread(&bmfh, sizeof(BITMAPFILEHEADER), 1, fp) != 1) {
		fclose(fp);
		return;
	}

	// BMP �������� ��Ÿ���� 'BM' ��Ŀ�� �ִ��� Ȯ��
	if (bmfh.bfType != DIB_HEADER_MARKER) {
		fclose(fp);
		return;
	}

	// BITMAPINFOHEADER �б�
	if (fread(&bmih, sizeof(BITMAPINFOHEADER), 1, fp) != 1) {
		fclose(fp);
		return;
	}

	LONG nWidth = bmih.biWidth;
	LONG nHeight = bmih.biHeight;
	WORD nBitCount = bmih.biBitCount;

	DWORD dwWidthStep = (DWORD)((nWidth * nBitCount / 8 + 3) & ~3);
	DWORD dwSizeImage = nHeight * dwWidthStep;

	// DIB ���� ��ü ũ�� ��� (BITMAPINFOHEADER + ���� ���̺� + �ȼ� ������)
	DWORD dwDibSize;
	if (nBitCount == 24)
		dwDibSize = sizeof(BITMAPINFOHEADER)+dwSizeImage;
	else
		dwDibSize = sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)* (1 << nBitCount) + dwSizeImage;

	// ���Ϸκ��� DIB ������ �о ������ �޸� ���� �Ҵ�
	BYTE* pDib = new BYTE[dwDibSize];

	if (pDib == NULL)
	{
		fclose(fp);
		return;
	}

	// ���Ϸκ��� Packed-DIB ũ�⸸ŭ�� �б�
	fseek(fp, sizeof(BITMAPFILEHEADER), SEEK_SET);
	if (fread(pDib, sizeof(BYTE), dwDibSize, fp) != dwDibSize)
	{
		delete[] pDib;
		pDib = NULL;
		fclose(fp);
		return;
	}

	// �ȼ� ������ ���� ��ġ ���
	LPVOID lpvBits;
	if (nBitCount == 24)
		lpvBits = pDib + sizeof(BITMAPINFOHEADER);
	else
		lpvBits = pDib + sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)* (1 << nBitCount);

	// DIB ȭ�� ���
	CClientDC dc(this);
	::SetDIBitsToDevice(dc.m_hDC, point.x, point.y, nWidth, nHeight, 0, 0, 0,
		nHeight, lpvBits, (BITMAPINFO*)pDib, DIB_RGB_COLORS);

	// �޸� ���� �� ���� �ݱ�
	delete[] pDib;
	fclose(fp);

	CView::OnLButtonDown(nFlags, point);
}
