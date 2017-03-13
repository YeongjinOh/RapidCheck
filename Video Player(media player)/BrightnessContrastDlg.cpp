// BrightnessContrastDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "BrightnessContrastDlg.h"
#include "afxdialogex.h"


// CBrightnessContrastDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CBrightnessContrastDlg, CDialogEx)

CBrightnessContrastDlg::CBrightnessContrastDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BRIGHTNESS_CONTRAST, pParent)
	, m_nBrightness(0)
	, m_nContrast(0)
{

}

CBrightnessContrastDlg::~CBrightnessContrastDlg()
{
}

void CBrightnessContrastDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BRIGHTNESS_SLIDER, m_sliderBrightness);
	DDX_Text(pDX, IDC_BRIGHTNESS_EDIT, m_nBrightness);
	DDV_MinMaxInt(pDX, m_nBrightness, -255, 255);
	DDX_Control(pDX, IDC_CONTRAST_SLIDER, m_sliderContrast);
	DDX_Text(pDX, IDC_CONTRAST_EDIT, m_nContrast);
	DDV_MinMaxInt(pDX, m_nContrast, -100, 100);
}


BEGIN_MESSAGE_MAP(CBrightnessContrastDlg, CDialogEx)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_BRIGHTNESS_EDIT, &CBrightnessContrastDlg::OnEnChangeBrightnessEdit)
	ON_EN_CHANGE(IDC_CONTRAST_EDIT, &CBrightnessContrastDlg::OnEnChangeContrastEdit)
END_MESSAGE_MAP()


// CBrightnessContrastDlg �޽��� ó�����Դϴ�.


BOOL CBrightnessContrastDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��� ���� �����̴� ��Ʈ�� �ʱ�ȭ
	m_sliderBrightness.SetRange(-255, 255);
	m_sliderBrightness.SetTicFreq(32);
	m_sliderBrightness.SetPageSize(32);

	// ��Ϻ� ���� �����̴� ��Ʈ�� �ʱ�ȭ
	m_sliderContrast.SetRange(-100, 100);
	m_sliderContrast.SetTicFreq(20);
	m_sliderContrast.SetPageSize(20);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CBrightnessContrastDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// ��� ���� �����̵�ٿ��� �߻��� WM_HSCROLL �޽��� ó��
	if (m_sliderBrightness.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		m_nBrightness = m_sliderBrightness.GetPos();
		UpdateData(FALSE);
	}
	// ��Ϻ� ���� �����̵�ٿ��� �߻��� WM_HSCROLL �޽��� ó��
	else if (m_sliderContrast.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		m_nContrast = m_sliderContrast.GetPos();
		UpdateData(FALSE);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CBrightnessContrastDlg::OnEnChangeBrightnessEdit()
{
	UpdateData(TRUE);
	m_sliderBrightness.SetPos(m_nBrightness);
}


void CBrightnessContrastDlg::OnEnChangeContrastEdit()
{
	UpdateData(TRUE);
	m_sliderContrast.SetPos(m_nContrast);
}
