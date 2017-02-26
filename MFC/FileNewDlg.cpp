// FileNewDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCtest.h"
#include "FileNewDlg.h"
#include "afxdialogex.h"


// CFileNewDlg 대화 상자입니다.

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


// CFileNewDlg 메시지 처리기입니다.
