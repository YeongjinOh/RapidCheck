
// MFCtest.h : MFCtest ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMFCtestApp:
// �� Ŭ������ ������ ���ؼ��� MFCtest.cpp�� �����Ͻʽÿ�.
//

class CMFCtestApp : public CWinApp
{
public:
	CMFCtestApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFCtestApp theApp;
