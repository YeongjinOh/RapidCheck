// FileNewDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCtest.h"
#include "FileNewDlg.h"
#include "afxdialogex.h"


// CFileNewDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CFileNewDlg, CDialogEx)

CFileNewDlg::CFileNewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFileNewDlg::IDD, pParent)
	, m_nWidth(0)
	, m_nType(0)
	, m_nHeight(0)
{

}

CFileNewDlg::~CFileNewDlg()
{
}

void CFileNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WIDTH, m_nWidth);
	DDV_MinMaxInt(pDX, m_nWidth, 1, 4096);
	DDX_CBIndex(pDX, IDC_IMG_TYPE, m_nType);
	DDX_Text(pDX, IDC_HEIGHT, m_nHeight);
}


BEGIN_MESSAGE_MAP(CFileNewDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileNewDlg �޽��� ó�����Դϴ�.
