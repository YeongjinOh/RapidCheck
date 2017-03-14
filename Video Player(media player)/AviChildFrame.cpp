// AviChildFrame.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "AviChildFrame.h"


// CAviChildFrame

IMPLEMENT_DYNCREATE(CAviChildFrame, CMDIChildWndEx)

CAviChildFrame::CAviChildFrame()
{

}

CAviChildFrame::~CAviChildFrame()
{
}


BEGIN_MESSAGE_MAP(CAviChildFrame, CMDIChildWndEx)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CAviChildFrame �޽��� ó�����Դϴ�.




int CAviChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwCtrlStyle = TBSTYLE_FLAT | TBSTYLE_TOOLTIPS | CBRS_SIZE_DYNAMIC;
	DWORD dwStyle = AFX_DEFAULT_TOOLBAR_STYLE;
	const CRect rcBorders(1, 1, 1, 1);
	if (!m_wndToolBar.CreateEx(this, dwCtrlStyle, dwStyle, rcBorders, IDR_AVI_TYPE) ||
		!m_wndToolBar.LoadToolBar(IDR_AVI_TYPE))
	{
		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	m_wndToolBar.SetWindowText(_T("Video Control"));
	DockPane(&m_wndToolBar);

	return 0;
}
