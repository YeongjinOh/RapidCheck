#pragma once


// CFileNewDlg ��ȭ �����Դϴ�.

class CFileNewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileNewDlg)

public:
	CFileNewDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CFileNewDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILE_NEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	// �̹��� ����, ���� ũ��
	int m_nWidth;
	int m_nHeight;

	// �̹��� ���� Ÿ�� (0: �׷��̽�����, 1: Ʈ���÷�)
	int m_nType;
};
