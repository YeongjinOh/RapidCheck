#pragma once


// CAddNoiseDlg ��ȭ �����Դϴ�.

class CAddNoiseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddNoiseDlg)

public:
	CAddNoiseDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAddNoiseDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADD_NOISE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int m_nNoiseType;
	int m_nAmount;

	virtual BOOL OnInitDialog();
};
