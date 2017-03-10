#pragma once
#include "afxcmn.h"


// CBrightnessContrastDlg ��ȭ �����Դϴ�.

class CBrightnessContrastDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBrightnessContrastDlg)

public:
	CBrightnessContrastDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CBrightnessContrastDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BRIGHTNESS_CONTRAST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	// ��� ��ȭ��
	CSliderCtrl m_sliderBrightness;
	int m_nBrightness;

	// ��Ϻ� ��ȭ�� (%)
	CSliderCtrl m_sliderContrast;
	int m_nContrast;

	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeBrightnessEdit();
	afx_msg void OnEnChangeContrastEdit();
};
