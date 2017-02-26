// CannyEdgeDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "CannyEdgeDlg.h"
#include "afxdialogex.h"


// CCannyEdgeDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCannyEdgeDlg, CDialogEx)

CCannyEdgeDlg::CCannyEdgeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CANNY_EDGE, pParent)
	, m_fSigma(1.4f)
	, m_fLowTh(30.f)
	, m_fHighTh(60.f)
{

}

CCannyEdgeDlg::~CCannyEdgeDlg()
{
}

void CCannyEdgeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CANNY_SIGMA, m_fSigma);
	DDX_Text(pDX, IDC_LOW_THRESHOLD, m_fLowTh);
	DDX_Text(pDX, IDC_HIGH_THRESHOLD, m_fHighTh);
}


BEGIN_MESSAGE_MAP(CCannyEdgeDlg, CDialogEx)
END_MESSAGE_MAP()


// CCannyEdgeDlg �޽��� ó�����Դϴ�.
