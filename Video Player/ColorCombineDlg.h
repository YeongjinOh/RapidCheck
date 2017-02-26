#pragma once
#include "afxwin.h"


// CColorCombineDlg ��ȭ �����Դϴ�.

class CColorCombineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CColorCombineDlg)

public:
	CColorCombineDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CColorCombineDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLOR_COMBINE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strColorSpace;
	CComboBox m_comboImage1;
	CComboBox m_comboImage2;
	CComboBox m_comboImage3;

	void* m_pDoc1;
	void* m_pDoc2;
	void* m_pDoc3;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
