#pragma once


// CRotateDlg ��ȭ �����Դϴ�.

class CRotateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRotateDlg)

public:
	CRotateDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CRotateDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROTATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int m_nRotate;
	float m_fAngle;

	afx_msg void OnEnSetfocusAngle();
	afx_msg void OnBnClickedRotate4();
};
