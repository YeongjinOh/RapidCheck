#pragma once


// CFourierDescDlg ��ȭ �����Դϴ�.

class CFourierDescDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFourierDescDlg)

public:
	CFourierDescDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CFourierDescDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FOURIER_DESCRIPTOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int m_nPercent;
};
