#pragma once


// CHarrisCornerDlg ��ȭ �����Դϴ�.

class CHarrisCornerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHarrisCornerDlg)

public:
	CHarrisCornerDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CHarrisCornerDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HARRIS_CORNER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int m_nHarrisTh;
};
