// FourierDescDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "FourierDescDlg.h"
#include "afxdialogex.h"


// CFourierDescDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CFourierDescDlg, CDialogEx)

CFourierDescDlg::CFourierDescDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FOURIER_DESCRIPTOR, pParent)
	, m_nPercent(100)
{

}

CFourierDescDlg::~CFourierDescDlg()
{
}

void CFourierDescDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PERCENT, m_nPercent);
	DDV_MinMaxInt(pDX, m_nPercent, 0, 100);
}


BEGIN_MESSAGE_MAP(CFourierDescDlg, CDialogEx)
END_MESSAGE_MAP()


// CFourierDescDlg �޽��� ó�����Դϴ�.
