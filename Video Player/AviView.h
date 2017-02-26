#pragma once

#include "IppImage\IppAvi.h"

// CAviView ���Դϴ�.

class CAviView : public CScrollView
{
	DECLARE_DYNCREATE(CAviView)

protected:
	CAviView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CAviView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual void OnInitialUpdate();     // ������ �� ó���Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	IppAvi m_Avi;
	int m_nCurrentFrame;	// ���� ��� ��ġ
	BOOL m_bPlay;
	int m_nMode;	// ��� ���

	BOOL AviFileOpen(LPCTSTR lpszPathName);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnVideoPlay();
	afx_msg void OnUpdateVideoPlay(CCmdUI *pCmdUI);
	afx_msg void OnVideoStop();
	afx_msg void OnUpdateVideoStop(CCmdUI *pCmdUI);
	afx_msg void OnVideoPause();
	afx_msg void OnUpdateVideoPause(CCmdUI *pCmdUI);
	afx_msg void OnVideoStart();
	afx_msg void OnVideoPrev();
	afx_msg void OnVideoNext();
	afx_msg void OnVideoEnd();
	afx_msg void OnVideoCapture();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnModeNormal();
	afx_msg void OnUpdateModeNormal(CCmdUI *pCmdUI);
	afx_msg void OnModeDifference();
	afx_msg void OnUpdateModeDifference(CCmdUI *pCmdUI);
	afx_msg void OnModeMotion();
	afx_msg void OnUpdateModeMotion(CCmdUI *pCmdUI);
};


