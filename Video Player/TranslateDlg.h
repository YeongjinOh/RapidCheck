#pragma once


// CTranslateDlg ��ȭ �����Դϴ�.

class CTranslateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTranslateDlg)

public:
	CTranslateDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTranslateDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRANSLATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int m_nNewSX;
	int m_nNewSY;
};
