// FreqFilteringDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "FreqFilteringDlg.h"
#include "afxdialogex.h"


// CFreqFilteringDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CFreqFilteringDlg, CDialogEx)

CFreqFilteringDlg::CFreqFilteringDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FREQUENCY_FILTERING, pParent)
	, m_nFilterType(0)
	, m_nFilterShape(0)
	, m_nCutoff(32)
	, m_strRange(_T(""))
{

}

CFreqFilteringDlg::~CFreqFilteringDlg()
{
}

void CFreqFilteringDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_FILTER_TYPE, m_nFilterType);
	DDX_CBIndex(pDX, IDC_FILTER_SHAPE, m_nFilterShape);
	DDX_Text(pDX, IDC_CUTOFF_FREQ, m_nCutoff);
	DDX_Text(pDX, IDC_CUTOFF_RANGE, m_strRange);
}


BEGIN_MESSAGE_MAP(CFreqFilteringDlg, CDialogEx)
END_MESSAGE_MAP()


// CFreqFilteringDlg �޽��� ó�����Դϴ�.
