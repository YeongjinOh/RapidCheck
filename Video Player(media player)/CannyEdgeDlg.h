#pragma once


// CCannyEdgeDlg ��ȭ �����Դϴ�.

class CCannyEdgeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCannyEdgeDlg)

public:
	CCannyEdgeDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCannyEdgeDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CANNY_EDGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	float m_fSigma;
	float m_fLowTh;
	float m_fHighTh;
};
