#pragma once

#include "stdafx.h"
#include "MFCtest.h"
#include "FileNewDlg.h"
#include "afxdialogex.h"


// CFileNewDlg ��ȭ �����Դϴ�.

class CFileNewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileNewDlg)

public:
	CFileNewDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CFileNewDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_FILE_NEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int m_nWidth;
	int m_nType;
	int m_nHeight;
};
