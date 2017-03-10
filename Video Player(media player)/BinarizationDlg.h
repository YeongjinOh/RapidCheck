#pragma once
#include "afxcmn.h"

#include "./IppImage/IppDib.h"

// CBinarizationDlg ��ȭ �����Դϴ�.

class CBinarizationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBinarizationDlg)

public:
	CBinarizationDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CBinarizationDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BINARIZATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_sliderThreshold;
	int m_nThreshold;

	IppDib m_DibSrc; // �Է� ������ ��� ���纻
	IppDib m_DibRes; // m_nThreshold�� �̿��Ͽ� m_DibSrc�� ����ȭ�� ����
	void SetImage(IppDib& dib);
	void MakePreviewImage();

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeThresholdEdit();
};
