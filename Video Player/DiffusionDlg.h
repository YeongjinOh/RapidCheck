#pragma once


// CDiffusionDlg ��ȭ �����Դϴ�.

class CDiffusionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDiffusionDlg)

public:
	CDiffusionDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDiffusionDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIFFUSION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	float m_fLambda;
	float m_fK;
	int m_nIteration;
};
