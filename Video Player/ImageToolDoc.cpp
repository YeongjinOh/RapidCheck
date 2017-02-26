
// ImageToolDoc.cpp : CImageToolDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "ImageTool.h"
#endif

#include "ImageToolDoc.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <algorithm>
#include <functional>

#include <propkey.h>

#include "IppImage\IppImage.h"
#include "IppImage\IppConvert.h"
#include "IppImage\IppEnhance.h"
#include "IppImage\IppFilter.h"
#include "IppImage\IppGeometry.h"
#include "IppImage\IppFourier.h"
#include "IppImage\IppFeature.h"
#include "IppImage\IppColor.h"
#include "IppImage\IppSegment.h"

#include "FileNewDlg.h"
#include "BrightnessContrastDlg.h"
#include "GammaCorrectionDlg.h"
#include "HistogramDlg.h"
#include "ArithmeticLogicalDlg.h"
#include "GaussianDlg.h"
#include "AddNoiseDlg.h"
#include "DiffusionDlg.h"
#include "TranslateDlg.h"
#include "ResizeDlg.h"
#include "RotateDlg.h"
#include "FreqFilteringDlg.h"
#include "CannyEdgeDlg.h"
#include "HarrisCornerDlg.h"
#include "ColorCombineDlg.h"
#include "BinarizationDlg.h"
#include "FourierDescDlg.h"

#define CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img) \
	IppByteImage img; \
	IppDibToImage(m_Dib, img);

#define CONVERT_DIB_TO_RGBIMAGE(m_Dib, img) \
	IppRgbImage img; \
	IppDibToImage(m_Dib, img);

#define CONVERT_IMAGE_TO_DIB(img, dib) \
	IppDib dib; \
	IppImageToDib(img, dib);

#define SHOW_SPECTRUM_PHASE_IMAGE

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CImageToolDoc

IMPLEMENT_DYNCREATE(CImageToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageToolDoc, CDocument)
	ON_COMMAND(ID_WINDOW_DUPLICATE, &CImageToolDoc::OnWindowDuplicate)
	ON_COMMAND(ID_EDIT_COPY, &CImageToolDoc::OnEditCopy)
	ON_COMMAND(ID_IMAGE_INVERSE, &CImageToolDoc::OnImageInverse)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_INVERSE, &CImageToolDoc::OnUpdateImageInverse)
	ON_COMMAND(ID_BRIGHTNESS_CONTRAST, &CImageToolDoc::OnBrightnessContrast)
	ON_UPDATE_COMMAND_UI(ID_BRIGHTNESS_CONTRAST, &CImageToolDoc::OnUpdateBrightnessContrast)
	ON_COMMAND(ID_GAMMA_CORRECTION, &CImageToolDoc::OnGammaCorrection)
	ON_UPDATE_COMMAND_UI(ID_GAMMA_CORRECTION, &CImageToolDoc::OnUpdateGammaCorrection)
	ON_COMMAND(ID_VIEW_HISTOGRAM, &CImageToolDoc::OnViewHistogram)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HISTOGRAM, &CImageToolDoc::OnUpdateViewHistogram)
	ON_COMMAND(ID_HISTO_STRETCHING, &CImageToolDoc::OnHistoStretching)
	ON_UPDATE_COMMAND_UI(ID_HISTO_STRETCHING, &CImageToolDoc::OnUpdateHistoStretching)
	ON_COMMAND(ID_HISTO_EQUALIZATION, &CImageToolDoc::OnHistoEqualization)
	ON_UPDATE_COMMAND_UI(ID_HISTO_EQUALIZATION, &CImageToolDoc::OnUpdateHistoEqualization)
	ON_COMMAND(ID_ARITHMETIC_LOGICAL, &CImageToolDoc::OnArithmeticLogical)
	ON_COMMAND(ID_BITPLANE_SLICING, &CImageToolDoc::OnBitplaneSlicing)
	ON_UPDATE_COMMAND_UI(ID_BITPLANE_SLICING, &CImageToolDoc::OnUpdateBitplaneSlicing)
	ON_COMMAND(ID_FILTER_MEAN, &CImageToolDoc::OnFilterMean)
	ON_UPDATE_COMMAND_UI(ID_FILTER_MEAN, &CImageToolDoc::OnUpdateFilterMean)
	ON_COMMAND(ID_FILTER_WEIGHTED_MEAN, &CImageToolDoc::OnFilterWeightedMean)
	ON_UPDATE_COMMAND_UI(ID_FILTER_WEIGHTED_MEAN, &CImageToolDoc::OnUpdateFilterWeightedMean)
	ON_COMMAND(ID_FILTER_GAUSSIAN, &CImageToolDoc::OnFilterGaussian)
	ON_UPDATE_COMMAND_UI(ID_FILTER_GAUSSIAN, &CImageToolDoc::OnUpdateFilterGaussian)
	ON_COMMAND(ID_FILTER_LAPLACIAN, &CImageToolDoc::OnFilterLaplacian)
	ON_UPDATE_COMMAND_UI(ID_FILTER_LAPLACIAN, &CImageToolDoc::OnUpdateFilterLaplacian)
	ON_COMMAND(ID_FILTER_UNSHARP_MASK, &CImageToolDoc::OnFilterUnsharpMask)
	ON_UPDATE_COMMAND_UI(ID_FILTER_UNSHARP_MASK, &CImageToolDoc::OnUpdateFilterUnsharpMask)
	ON_COMMAND(ID_FILTER_HIGHBOOST, &CImageToolDoc::OnFilterHighboost)
	ON_UPDATE_COMMAND_UI(ID_FILTER_HIGHBOOST, &CImageToolDoc::OnUpdateFilterHighboost)
	ON_COMMAND(ID_ADD_NOISE, &CImageToolDoc::OnAddNoise)
	ON_UPDATE_COMMAND_UI(ID_ADD_NOISE, &CImageToolDoc::OnUpdateAddNoise)
	ON_COMMAND(ID_FILTER_MEDIAN, &CImageToolDoc::OnFilterMedian)
	ON_UPDATE_COMMAND_UI(ID_FILTER_MEDIAN, &CImageToolDoc::OnUpdateFilterMedian)
	ON_COMMAND(ID_FILTER_DIFFUSION, &CImageToolDoc::OnFilterDiffusion)
	ON_UPDATE_COMMAND_UI(ID_FILTER_DIFFUSION, &CImageToolDoc::OnUpdateFilterDiffusion)
	ON_COMMAND(ID_IMAGE_TRANSLATION, &CImageToolDoc::OnImageTranslation)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_TRANSLATION, &CImageToolDoc::OnUpdateImageTranslation)
	ON_COMMAND(ID_IMAGE_RESIZE, &CImageToolDoc::OnImageResize)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_RESIZE, &CImageToolDoc::OnUpdateImageResize)
	ON_COMMAND(ID_IMAGE_ROTATE, &CImageToolDoc::OnImageRotate)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_ROTATE, &CImageToolDoc::OnUpdateImageRotate)
	ON_COMMAND(ID_IMAGE_MIRROR, &CImageToolDoc::OnImageMirror)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_MIRROR, &CImageToolDoc::OnUpdateImageMirror)
	ON_COMMAND(ID_IMAGE_FLIP, &CImageToolDoc::OnImageFlip)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_FLIP, &CImageToolDoc::OnUpdateImageFlip)
	ON_COMMAND(ID_FOURIER_DFT, &CImageToolDoc::OnFourierDft)
	ON_UPDATE_COMMAND_UI(ID_FOURIER_DFT, &CImageToolDoc::OnUpdateFourierDft)
	ON_COMMAND(ID_FOURIER_DFTRC, &CImageToolDoc::OnFourierDftrc)
	ON_UPDATE_COMMAND_UI(ID_FOURIER_DFTRC, &CImageToolDoc::OnUpdateFourierDftrc)
	ON_COMMAND(ID_FOURIER_FFT, &CImageToolDoc::OnFourierFft)
	ON_UPDATE_COMMAND_UI(ID_FOURIER_FFT, &CImageToolDoc::OnUpdateFourierFft)
	ON_COMMAND(ID_FREQ_FILTERING, &CImageToolDoc::OnFreqFiltering)
	ON_UPDATE_COMMAND_UI(ID_FREQ_FILTERING, &CImageToolDoc::OnUpdateFreqFiltering)
	ON_COMMAND(ID_EDGE_ROBERTS, &CImageToolDoc::OnEdgeRoberts)
	ON_UPDATE_COMMAND_UI(ID_EDGE_ROBERTS, &CImageToolDoc::OnUpdateEdgeRoberts)
	ON_COMMAND(ID_EDGE_PREWITT, &CImageToolDoc::OnEdgePrewitt)
	ON_UPDATE_COMMAND_UI(ID_EDGE_PREWITT, &CImageToolDoc::OnUpdateEdgePrewitt)
	ON_COMMAND(ID_EDGE_SOBEL, &CImageToolDoc::OnEdgeSobel)
	ON_UPDATE_COMMAND_UI(ID_EDGE_SOBEL, &CImageToolDoc::OnUpdateEdgeSobel)
	ON_COMMAND(ID_EDGE_CANNY, &CImageToolDoc::OnEdgeCanny)
	ON_UPDATE_COMMAND_UI(ID_EDGE_CANNY, &CImageToolDoc::OnUpdateEdgeCanny)
	ON_COMMAND(ID_HOUGH_LINE, &CImageToolDoc::OnHoughLine)
	ON_UPDATE_COMMAND_UI(ID_HOUGH_LINE, &CImageToolDoc::OnUpdateHoughLine)
	ON_COMMAND(ID_HARRIS_CORNER, &CImageToolDoc::OnHarrisCorner)
	ON_UPDATE_COMMAND_UI(ID_HARRIS_CORNER, &CImageToolDoc::OnUpdateHarrisCorner)
	ON_COMMAND(ID_COLOR_GRAYSCALE, &CImageToolDoc::OnColorGrayscale)
	ON_UPDATE_COMMAND_UI(ID_COLOR_GRAYSCALE, &CImageToolDoc::OnUpdateColorGrayscale)
	ON_COMMAND(ID_COLOR_SPLIT_RGB, &CImageToolDoc::OnColorSplitRgb)
	ON_UPDATE_COMMAND_UI(ID_COLOR_SPLIT_RGB, &CImageToolDoc::OnUpdateColorSplitRgb)
	ON_COMMAND(ID_COLOR_SPLIT_HSI, &CImageToolDoc::OnColorSplitHsi)
	ON_UPDATE_COMMAND_UI(ID_COLOR_SPLIT_HSI, &CImageToolDoc::OnUpdateColorSplitHsi)
	ON_COMMAND(ID_COLOR_SPLIT_YUV, &CImageToolDoc::OnColorSplitYuv)
	ON_UPDATE_COMMAND_UI(ID_COLOR_SPLIT_YUV, &CImageToolDoc::OnUpdateColorSplitYuv)
	ON_COMMAND(ID_COLOR_COMBINE_RGB, &CImageToolDoc::OnColorCombineRgb)
	ON_COMMAND(ID_COLOR_COMBINE_HSI, &CImageToolDoc::OnColorCombineHsi)
	ON_COMMAND(ID_COLOR_COMBINE_YUV, &CImageToolDoc::OnColorCombineYuv)
	ON_COMMAND(ID_COLOR_EDGE, &CImageToolDoc::OnColorEdge)
	ON_UPDATE_COMMAND_UI(ID_COLOR_EDGE, &CImageToolDoc::OnUpdateColorEdge)
	ON_COMMAND(ID_SEGMENT_BINARIZATION, &CImageToolDoc::OnSegmentBinarization)
	ON_UPDATE_COMMAND_UI(ID_SEGMENT_BINARIZATION, &CImageToolDoc::OnUpdateSegmentBinarization)
	ON_COMMAND(ID_SEGMENT_LABELING, &CImageToolDoc::OnSegmentLabeling)
	ON_UPDATE_COMMAND_UI(ID_SEGMENT_LABELING, &CImageToolDoc::OnUpdateSegmentLabeling)
	ON_COMMAND(ID_CONTOUR_TRACING, &CImageToolDoc::OnContourTracing)
	ON_UPDATE_COMMAND_UI(ID_CONTOUR_TRACING, &CImageToolDoc::OnUpdateContourTracing)
	ON_COMMAND(ID_MORPHOLOGY_EROSION, &CImageToolDoc::OnMorphologyErosion)
	ON_UPDATE_COMMAND_UI(ID_MORPHOLOGY_EROSION, &CImageToolDoc::OnUpdateMorphologyErosion)
	ON_COMMAND(ID_MORPHOLOGY_DILATION, &CImageToolDoc::OnMorphologyDilation)
	ON_UPDATE_COMMAND_UI(ID_MORPHOLOGY_DILATION, &CImageToolDoc::OnUpdateMorphologyDilation)
	ON_COMMAND(ID_MORPHOLOGY_OPENING, &CImageToolDoc::OnMorphologyOpening)
	ON_UPDATE_COMMAND_UI(ID_MORPHOLOGY_OPENING, &CImageToolDoc::OnUpdateMorphologyOpening)
	ON_COMMAND(ID_MORPHOLOGY_CLOSING, &CImageToolDoc::OnMorphologyClosing)
	ON_UPDATE_COMMAND_UI(ID_MORPHOLOGY_CLOSING, &CImageToolDoc::OnUpdateMorphologyClosing)
	ON_COMMAND(ID_GRAYMORPH_EROSION, &CImageToolDoc::OnGraymorphErosion)
	ON_UPDATE_COMMAND_UI(ID_GRAYMORPH_EROSION, &CImageToolDoc::OnUpdateGraymorphErosion)
	ON_COMMAND(ID_GRAYMORPH_DILATION, &CImageToolDoc::OnGraymorphDilation)
	ON_UPDATE_COMMAND_UI(ID_GRAYMORPH_DILATION, &CImageToolDoc::OnUpdateGraymorphDilation)
	ON_COMMAND(ID_GRAYMORPH_OPENING, &CImageToolDoc::OnGraymorphOpening)
	ON_UPDATE_COMMAND_UI(ID_GRAYMORPH_OPENING, &CImageToolDoc::OnUpdateGraymorphOpening)
	ON_COMMAND(ID_GRAYMORPH_CLOSING, &CImageToolDoc::OnGraymorphClosing)
	ON_UPDATE_COMMAND_UI(ID_GRAYMORPH_CLOSING, &CImageToolDoc::OnUpdateGraymorphClosing)
	ON_COMMAND(ID_FOURIER_DESCRIPTOR, &CImageToolDoc::OnFourierDescriptor)
	ON_UPDATE_COMMAND_UI(ID_FOURIER_DESCRIPTOR, &CImageToolDoc::OnUpdateFourierDescriptor)
	ON_COMMAND(ID_INVARIANT_MOMENTS, &CImageToolDoc::OnInvariantMoments)
	ON_UPDATE_COMMAND_UI(ID_INVARIANT_MOMENTS, &CImageToolDoc::OnUpdateInvariantMoments)
	ON_COMMAND(ID_ZERNIKE_MOMENTS, &CImageToolDoc::OnZernikeMoments)
	ON_UPDATE_COMMAND_UI(ID_ZERNIKE_MOMENTS, &CImageToolDoc::OnUpdateZernikeMoments)
	ON_COMMAND(ID_TEMPLATE_MATCHING, &CImageToolDoc::OnTemplateMatching)
	ON_UPDATE_COMMAND_UI(ID_TEMPLATE_MATCHING, &CImageToolDoc::OnUpdateTemplateMatching)
END_MESSAGE_MAP()


// CImageToolDoc ����/�Ҹ�

CImageToolDoc::CImageToolDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CImageToolDoc::~CImageToolDoc()
{
}

BOOL CImageToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	BOOL ret = TRUE;

	if (theApp.m_pNewDib == NULL)
	{
		CFileNewDlg dlg;
		if (dlg.DoModal() == IDOK)
		{
			if (dlg.m_nType == 0) // �׷��̽����� ��Ʈ��
				ret = m_Dib.CreateGrayBitmap(dlg.m_nWidth, dlg.m_nHeight);
			else // Ʈ���÷� ��Ʈ��
				ret = m_Dib.CreateRgbBitmap(dlg.m_nWidth, dlg.m_nHeight);
		}
		else
		{
			ret = FALSE;
		}
	}
	else
	{
		m_Dib = *(theApp.m_pNewDib);
		theApp.m_pNewDib = NULL;
	}

	return ret;
}




// CImageToolDoc serialization

void CImageToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}

#ifdef SHARED_HANDLERS

// ����� �׸��� �����մϴ�.
void CImageToolDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// �˻� ó���⸦ �����մϴ�.
void CImageToolDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CImageToolDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CImageToolDoc ����

#ifdef _DEBUG
void CImageToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImageToolDoc ���


BOOL CImageToolDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	BOOL res = m_Dib.Load(CT2A(lpszPathName));
	if (res)
		AfxPrintInfo(_T("[���� ����] ���� ���: %s, ���� ũ��: %d�ȼ�, ���� ũ��: %d�ȼ�, �����: %d"),
			lpszPathName, m_Dib.GetWidth(), m_Dib.GetHeight(), 0x01 << m_Dib.GetBitCount());

	return res;
}


BOOL CImageToolDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	return m_Dib.Save(CT2A(lpszPathName));
}


void CImageToolDoc::OnWindowDuplicate()
{
	AfxNewBitmap(m_Dib);
}


void CImageToolDoc::OnEditCopy()
{
	if (m_Dib.IsValid())
		m_Dib.CopyToClipboard();
}


void CImageToolDoc::OnImageInverse()
{
	if (m_Dib.GetBitCount() == 8)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppInverse(img);
		CONVERT_IMAGE_TO_DIB(img, dib)

		AfxPrintInfo(_T("[����] �Է� ����: %s"), GetTitle());
		AfxNewBitmap(dib);
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
		IppInverse(img);
		CONVERT_IMAGE_TO_DIB(img, dib)

		AfxPrintInfo(_T("[����] �Է� ����: %s"), GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateImageInverse(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8 || m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnBrightnessContrast()
{
	CBrightnessContrastDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		if (m_Dib.GetBitCount() == 8)
		{
			CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
			IppBrightness(img, dlg.m_nBrightness);
			IppContrast(img, dlg.m_nContrast);
			CONVERT_IMAGE_TO_DIB(img, dib)

			AfxPrintInfo(_T("[���/��Ϻ� ����] �Է� ����: %s, ���: %d, ��Ϻ�: %d%%"),
				GetTitle(), dlg.m_nBrightness, dlg.m_nContrast);
			AfxNewBitmap(dib);
		}
		else if (m_Dib.GetBitCount() == 24)
		{
			CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
			IppByteImage imgY, imgU, imgV;
			IppColorSplitYUV(img, imgY, imgU, imgV);
			IppBrightness(imgY, dlg.m_nBrightness);
			IppContrast(imgY, dlg.m_nContrast);

			IppRgbImage imgRes;
			IppColorCombineYUV(imgY, imgU, imgV, imgRes);
			CONVERT_IMAGE_TO_DIB(imgRes, dib)

			AfxPrintInfo(_T("[���/��Ϻ� ����] �Է� ����: %s, ���: %d, ��Ϻ�: %d%%"),
				GetTitle(), dlg.m_nBrightness, dlg.m_nContrast);
			AfxNewBitmap(dib);
		}
	}
}


void CImageToolDoc::OnUpdateBrightnessContrast(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8 || m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnGammaCorrection()
{
	CGammaCorrectionDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		if (m_Dib.GetBitCount() == 8)
		{
			CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
			IppGammaCorrection(img, dlg.m_fGamma);
			CONVERT_IMAGE_TO_DIB(img, dib)

			AfxPrintInfo(_T("[���� ����] �Է� ����: %s, ����: %4.2f"), GetTitle(), dlg.m_fGamma);
			AfxNewBitmap(dib);
		}
		else if (m_Dib.GetBitCount() == 24)
		{
			CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
			IppByteImage imgY, imgU, imgV;
			IppColorSplitYUV(img, imgY, imgU, imgV);
			IppGammaCorrection(imgY, dlg.m_fGamma);

			IppRgbImage imgRes;
			IppColorCombineYUV(imgY, imgU, imgV, imgRes);
			CONVERT_IMAGE_TO_DIB(imgRes, dib)

			AfxPrintInfo(_T("[���� ����] �Է� ����: %s, ����: %4.2f"), GetTitle(), dlg.m_fGamma);
			AfxNewBitmap(dib);

		}
	}
}


void CImageToolDoc::OnUpdateGammaCorrection(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8 || m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnViewHistogram()
{
	CHistogramDlg dlg;
	dlg.SetImage(&m_Dib);
	dlg.DoModal();
}


void CImageToolDoc::OnUpdateViewHistogram(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnHistoStretching()
{
	if (m_Dib.GetBitCount() == 8)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppHistogramStretching(img);
		CONVERT_IMAGE_TO_DIB(img, dib)

		AfxPrintInfo(_T("[������׷� ��Ʈ��Ī] �Է� ����: %s"), GetTitle());
		AfxNewBitmap(dib);
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
		IppByteImage imgY, imgU, imgV;
		IppColorSplitYUV(img, imgY, imgU, imgV);
		IppHistogramStretching(imgY);

		IppRgbImage imgRes;
		IppColorCombineYUV(imgY, imgU, imgV, imgRes);
		CONVERT_IMAGE_TO_DIB(imgRes, dib)

		AfxPrintInfo(_T("[������׷� ��Ʈ��Ī] �Է� ����: %s"), GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateHistoStretching(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8 || m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnHistoEqualization()
{
	if (m_Dib.GetBitCount() == 8)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppHistogramEqualization(img);
		CONVERT_IMAGE_TO_DIB(img, dib)

		AfxPrintInfo(_T("[������׷� �յ�ȭ] �Է� ����: %s"), GetTitle());
		AfxNewBitmap(dib);
	}
	else if (m_Dib.GetBitCount() == 24)
	{
		CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
		IppByteImage imgY, imgU, imgV;
		IppColorSplitYUV(img, imgY, imgU, imgV);
		IppHistogramEqualization(imgY);

		IppRgbImage imgRes;
		IppColorCombineYUV(imgY, imgU, imgV, imgRes);
		CONVERT_IMAGE_TO_DIB(imgRes, dib)

		AfxPrintInfo(_T("[������׷� �յ�ȭ] �Է� ����: %s"), GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateHistoEqualization(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8 || m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnArithmeticLogical()
{
	CArithmeticLogicalDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CImageToolDoc* pDoc1 = (CImageToolDoc*)dlg.m_pDoc1;
		CImageToolDoc* pDoc2 = (CImageToolDoc*)dlg.m_pDoc2;

		CONVERT_DIB_TO_BYTEIMAGE(pDoc1->m_Dib, img1)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc2->m_Dib, img2)
		IppByteImage img3;

		bool ret = false;

		switch (dlg.m_nFunction)
		{
		case 0: ret = IppAdd(img1, img2, img3);  break;
		case 1: ret = IppSub(img1, img2, img3);  break;
		case 2: ret = IppAve(img1, img2, img3);  break;
		case 3: ret = IppDiff(img1, img2, img3); break;
		case 4: ret = IppAND(img1, img2, img3);  break;
		case 5: ret = IppOR(img1, img2, img3);   break;
		}

		if (ret)
		{
			CONVERT_IMAGE_TO_DIB(img3, dib)

			TCHAR* op[] = { _T("����"), _T("����"), _T("���"), _T("����"), _T("�� AND"), _T("�� OR") };
			AfxPrintInfo(_T("[��� �� �� ����] [%s] �Է� ���� #1: %s, �Է� ���� #2: %s"), 
				op[dlg.m_nFunction], pDoc1->GetTitle(), pDoc2->GetTitle());
			AfxNewBitmap(dib);
		}
		else
			AfxMessageBox(_T("������ ũ�Ⱑ �ٸ��ϴ�!"));
	}
}


void CImageToolDoc::OnBitplaneSlicing()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgPlane;

	for (int i = 0; i < 8; i++)
	{
		IppBitPlane(img, imgPlane, i);
		CONVERT_IMAGE_TO_DIB(imgPlane, dib)
		AfxNewBitmap(dib);
	}

	AfxPrintInfo(_T("[��Ʈ ��� ����] �Է� ����: %s"), GetTitle());
}


void CImageToolDoc::OnUpdateBitplaneSlicing(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFilterMean()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppFilterMean(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[��� �� ����] �Է� ����: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateFilterMean(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFilterWeightedMean()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppFilterWeightedMean(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[���� ��� �� ����] �Է� ����: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateFilterWeightedMean(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFilterGaussian()
{
	CGaussianDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppFloatImage imgDst;
		IppFilterGaussian(imgSrc, imgDst, dlg.m_fSigma);
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxPrintInfo(_T("[����þ� ����] �Է� ����: %s, Sigma: %4.2f"), GetTitle(), dlg.m_fSigma);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateFilterGaussian(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFilterLaplacian()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppFilterLaplacian(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[���ö�þ� ����] �Է� ����: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateFilterLaplacian(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFilterUnsharpMask()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppFilterUnsharpMask(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[����� ����ũ ����] �Է� ����: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateFilterUnsharpMask(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFilterHighboost()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	float alpha = 1.2f;
	IppFilterHighboost(imgSrc, imgDst, alpha);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[���̺ν�Ʈ ����] �Է� ����: %s, alpha = %4.2f"), GetTitle(), alpha);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateFilterHighboost(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnAddNoise()
{
	CAddNoiseDlg dlg;
	if( dlg.DoModal() == IDOK )
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppByteImage imgDst;

		if( dlg.m_nNoiseType == 0 )
			IppNoiseGaussian(imgSrc, imgDst, dlg.m_nAmount);
		else
			IppNoiseSaltNPepper(imgSrc, imgDst, dlg.m_nAmount);

		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		TCHAR* noise[] = { _T("����þ�"), _T("�ұ�&����") };
		AfxPrintInfo(_T("[���� �߰�] �Է� ����: %s, ���� ����: %s, ���� ��: %d"), 
			GetTitle(), noise[dlg.m_nNoiseType], dlg.m_nAmount);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateAddNoise(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFilterMedian()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppFilterMedian(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[�̵�� ����] �Է� ����: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateFilterMedian(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFilterDiffusion()
{
	CDiffusionDlg dlg;
	if( dlg.DoModal() == IDOK )
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppFloatImage imgDst;
		IppFilterDiffusion(imgSrc, imgDst, dlg.m_fLambda, dlg.m_fK, dlg.m_nIteration);
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxPrintInfo(_T("[���漺 Ȯ�� ����] �Է� ����: %s, Lambda: %4.2f, K: %4.2f, �ݺ� Ƚ��: %d"), 
			GetTitle(), dlg.m_fLambda, dlg.m_fK, dlg.m_nIteration);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateFilterDiffusion(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnImageTranslation()
{
	CTranslateDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppByteImage imgDst;
		IppTranslate(imgSrc, imgDst, dlg.m_nNewSX, dlg.m_nNewSY);
		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		AfxPrintInfo(_T("[�̵� ��ȯ] �Է� ����: %s, ���� �̵�: %d, ���� �̵�: %d"),
			GetTitle(), dlg.m_nNewSX, dlg.m_nNewSY);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateImageTranslation(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnImageResize()
{
	CResizeDlg dlg;
	dlg.m_nOldWidth = m_Dib.GetWidth();
	dlg.m_nOldHeight = m_Dib.GetHeight();
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppByteImage imgDst;
		switch (dlg.m_nInterpolation)
		{
		case 0: IppResizeNearest(imgSrc, imgDst, dlg.m_nNewWidth, dlg.m_nNewHeight); break;
		case 1: IppResizeBilinear(imgSrc, imgDst, dlg.m_nNewWidth, dlg.m_nNewHeight); break;
		case 2: IppResizeCubic(imgSrc, imgDst, dlg.m_nNewWidth, dlg.m_nNewHeight); break;
		}

		CONVERT_IMAGE_TO_DIB(imgDst, dib)
		
		TCHAR* interpolation[] = { _T("�ֱٹ� �̿� ������"), _T("�缱�� ������"), _T("3�� ȸ�� ������") };
		AfxPrintInfo(_T("[ũ�� ��ȯ] �Է� ����: %s, , �� ���� ũ��: %d, �� ���� ũ��: %d, ������: %s"),
			GetTitle(), dlg.m_nNewWidth, dlg.m_nNewHeight, interpolation[dlg.m_nInterpolation]);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateImageResize(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnImageRotate()
{
	CRotateDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
		IppByteImage imgDst;
		switch (dlg.m_nRotate)
		{
		case 0: IppRotate90(imgSrc, imgDst); break;
		case 1: IppRotate180(imgSrc, imgDst); break;
		case 2: IppRotate270(imgSrc, imgDst); break;
		case 3: IppRotate(imgSrc, imgDst, (double)dlg.m_fAngle); break;
		}

		CONVERT_IMAGE_TO_DIB(imgDst, dib)

		TCHAR* rotate[] = { _T("90��"), _T("180��"), _T("270��") };
		if (dlg.m_nRotate != 3)
			AfxPrintInfo(_T("[ȸ�� ��ȯ] �Է� ����: %s, ȸ�� ����: %s"), GetTitle(), rotate[dlg.m_nRotate]);
		else
			AfxPrintInfo(_T("[ȸ�� ��ȯ] �Է� ����: %s, ȸ�� ����: %4.2f��"), GetTitle(), dlg.m_fAngle);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateImageRotate(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnImageMirror()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppMirror(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[�¿� ��Ī] �Է� ����: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateImageMirror(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnImageFlip()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, imgSrc)
	IppByteImage imgDst;
	IppFlip(imgSrc, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[���� ��Ī] �Է� ����: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateImageFlip(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFourierDft()
{
	int w = m_Dib.GetWidth();
	int h = m_Dib.GetHeight();

	if (w * h > 128 * 128)
	{
		CString msg = _T("������ ũ�Ⱑ Ŀ�� �ð��� ���� �ɸ� �� �ֽ��ϴ�.\n��� �Ͻðڽ��ϱ�?");
		if (AfxMessageBox(msg, MB_OKCANCEL) != IDOK)
			return;
	}

	CWaitCursor wait;

	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)

	IppFourier fourier;
	fourier.SetImage(img);

	DWORD t1 = timeGetTime();
	fourier.DFT(1);

#ifdef SHOW_SPECTRUM_PHASE_IMAGE
	IppByteImage imgSpec;
	fourier.GetSpectrumImage(imgSpec);

	CONVERT_IMAGE_TO_DIB(imgSpec, dibSpec)
	AfxNewBitmap(dibSpec);

	IppByteImage imgPhase;
	fourier.GetPhaseImage(imgPhase);

	CONVERT_IMAGE_TO_DIB(imgPhase, dibPhase)
	AfxNewBitmap(dibPhase);
#endif

	fourier.DFT(-1);
	DWORD t2 = timeGetTime();

	IppByteImage img2;
	fourier.GetImage(img2);

	CONVERT_IMAGE_TO_DIB(img2, dib)

	AfxPrintInfo(_T("[Ǫ������ȯ/DFT] �Է� ����: %s, �Է� ���� ũ��: %dx%d, ó�� �ð�: %dmsec"),
		GetTitle(), w, h, t2 - t1);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateFourierDft(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFourierDftrc()
{
	int w = m_Dib.GetWidth();
	int h = m_Dib.GetHeight();

	if (w * h > 256 * 256)
	{
		CString msg = _T("������ ũ�Ⱑ Ŀ�� �ð��� ���� �ɸ� �� �ֽ��ϴ�.\n��� �Ͻðڽ��ϱ�?");
		if (AfxMessageBox(msg, MB_OKCANCEL) != IDOK)
			return;
	}

	CWaitCursor wait;

	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)

	IppFourier fourier;
	fourier.SetImage(img);

	DWORD t1 = timeGetTime();
	fourier.DFTRC(1);

#ifdef SHOW_SPECTRUM_PHASE_IMAGE
	IppByteImage imgSpec;
	fourier.GetSpectrumImage(imgSpec);

	CONVERT_IMAGE_TO_DIB(imgSpec, dibSpec)
	AfxNewBitmap(dibSpec);

	IppByteImage imgPhase;
	fourier.GetPhaseImage(imgPhase);

	CONVERT_IMAGE_TO_DIB(imgPhase, dibPhase)
	AfxNewBitmap(dibPhase);
#endif

	fourier.DFTRC(-1);
	DWORD t2 = timeGetTime();

	IppByteImage img2;
	fourier.GetImage(img2);

	CONVERT_IMAGE_TO_DIB(img2, dib)

	AfxPrintInfo(_T("[Ǫ������ȯ/DFTRC] �Է� ����: %s, �Է� ���� ũ��: %dx%d, ó�� �ð�: %dmsec"),
		GetTitle(), w, h, t2 - t1);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateFourierDftrc(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFourierFft()
{
	int w = m_Dib.GetWidth();
	int h = m_Dib.GetHeight();

	if (!IsPowerOf2(w) || !IsPowerOf2(h))
	{
		AfxMessageBox(_T("���� �Ǵ� ������ ũ�Ⱑ 2�� �¼��� �ƴմϴ�."));
		return;
	}

	CWaitCursor wait;

	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)

	IppFourier fourier;
	fourier.SetImage(img);

	DWORD t1 = timeGetTime();
	fourier.FFT(1);

#ifdef SHOW_SPECTRUM_PHASE_IMAGE
	IppByteImage imgSpec;
	fourier.GetSpectrumImage(imgSpec);

	CONVERT_IMAGE_TO_DIB(imgSpec, dibSpec)
	AfxNewBitmap(dibSpec);

	IppByteImage imgPhase;
	fourier.GetPhaseImage(imgPhase);

	CONVERT_IMAGE_TO_DIB(imgPhase, dibPhase)
	AfxNewBitmap(dibPhase);
#endif

	fourier.FFT(-1);
	DWORD t2 = timeGetTime();

	IppByteImage img2;
	fourier.GetImage(img2);

	CONVERT_IMAGE_TO_DIB(img2, dib)

	AfxPrintInfo(_T("[Ǫ������ȯ/FFT] �Է� ����: %s, �Է� ���� ũ��: %dx%d, ó�� �ð�: %dmsec"),
		GetTitle(), w, h, t2 - t1);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateFourierFft(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFreqFiltering()
{
	int w = m_Dib.GetWidth();
	int h = m_Dib.GetHeight();

	if (!IsPowerOf2(w) || !IsPowerOf2(h))
	{
		AfxMessageBox(_T("���� �Ǵ� ������ ũ�Ⱑ 2�� �¼��� �ƴմϴ�."));
		return;
	}

	CFreqFilteringDlg dlg;
	dlg.m_strRange.Format(_T("(0 ~ %d)"), min(w / 2, h / 2));
	if (dlg.DoModal() == IDOK)
	{
		CWaitCursor wait;
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)

		IppFourier fourier;
		fourier.SetImage(img);
		fourier.FFT(1);

		if (dlg.m_nFilterType == 0)
		{
			if (dlg.m_nFilterShape == 0)
				fourier.LowPassIdeal(dlg.m_nCutoff);
			else
				fourier.LowPassGaussian(dlg.m_nCutoff);
		}
		else
		{
			if (dlg.m_nFilterShape == 0)
				fourier.HighPassIdeal(dlg.m_nCutoff);
			else
				fourier.HighPassGaussian(dlg.m_nCutoff);
		}

		fourier.FFT(-1);

		IppByteImage img2;
		fourier.GetImage(img2);
		CONVERT_IMAGE_TO_DIB(img2, dib)

		TCHAR* type[] = { _T("���� ��� ����"), _T("�� ��� ����") };
		TCHAR* shape[] = { _T("�̻���(Ideal)"), _T("����þ�(Gaussian)") };
		AfxPrintInfo(_T("[���ļ� ���� ���͸�] �Է� ����: %s, ���� ����: %s, ���� ���: %s, ���� ���ļ�: %d"),
			GetTitle(), type[dlg.m_nFilterType], shape[dlg.m_nFilterShape], dlg.m_nCutoff);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateFreqFiltering(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnEdgeRoberts()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgEdge;
	IppEdgeRoberts(img, imgEdge);
	CONVERT_IMAGE_TO_DIB(imgEdge, dib)

	AfxPrintInfo(_T("[����ũ ��� ���� ����/�ι���] �Է� ����: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateEdgeRoberts(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnEdgePrewitt()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgEdge;
	IppEdgePrewitt(img, imgEdge);
	CONVERT_IMAGE_TO_DIB(imgEdge, dib)

	AfxPrintInfo(_T("[����ũ ��� ���� ����/������] �Է� ����: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateEdgePrewitt(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnEdgeSobel()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgEdge;
	IppEdgeSobel(img, imgEdge);
	CONVERT_IMAGE_TO_DIB(imgEdge, dib)

	AfxPrintInfo(_T("[����ũ ��� ���� ����/�Һ�] �Է� ����: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateEdgeSobel(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnEdgeCanny()
{
	CCannyEdgeDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppByteImage imgEdge;
		IppEdgeCanny(img, imgEdge, dlg.m_fSigma, dlg.m_fLowTh, dlg.m_fHighTh);
		CONVERT_IMAGE_TO_DIB(imgEdge, dib)

		AfxPrintInfo(_T("[ĳ�� ���� ����] �Է� ����: %s, sigma: %4.2f, Low Th: %4.2f, High Th: %4.2f"), 
			GetTitle(), dlg.m_fSigma, dlg.m_fLowTh, dlg.m_fHighTh);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateEdgeCanny(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnHoughLine()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgEdge;
	IppEdgeCanny(img, imgEdge, 1.4f, 30.f, 60.f);

	std::vector<IppLineParam> lines;
	IppHoughLine(imgEdge, lines);

	if (lines.size() == 0)
	{
		AfxMessageBox(_T("����� ������ �����ϴ�."));
		return;
	}

	std::sort(lines.begin(), lines.end());

	// �ִ� 10���� ������ ȭ�鿡 �׷���.
	int cnt = min(10, lines.size());
	for (int i = 0; i < cnt; i++)
		IppDrawLine(img, lines[i], 255);

	CONVERT_IMAGE_TO_DIB(img, dib)

	AfxPrintInfo(_T("[���� �� ����] �Է� ����: %s, �߿� ����: rho = %4.2f, angle = %4.2f, vote = %d"),
		GetTitle(), lines[0].rho, (lines[0].ang*180/3.14f), lines[0].vote);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateHoughLine(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnHarrisCorner()
{
	CHarrisCornerDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		std::vector<IppPoint> corners;
		IppHarrisCorner(img, corners, dlg.m_nHarrisTh);

		BYTE** ptr = img.GetPixels2D();

		int x, y;
		for (IppPoint cp : corners)
		{
			x = cp.x;
			y = cp.y;

			ptr[y - 1][x - 1] = ptr[y - 1][x] = ptr[y - 1][x + 1] = 0;
			ptr[y][x - 1] = ptr[y][x] = ptr[y][x + 1] = 0;
			ptr[y + 1][x - 1] = ptr[y + 1][x] = ptr[y + 1][x + 1] = 0;
		}

		CONVERT_IMAGE_TO_DIB(img, dib)

		AfxPrintInfo(_T("[�ظ��� �ڳ� ����] �Է� ����: %s, Threshold: %d, ����� �ڳ� ����: %d"), 
			GetTitle(), dlg.m_nHarrisTh, corners.size());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateHarrisCorner(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnColorGrayscale()
{
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor)
	IppByteImage imgGray;
	imgGray.Convert(imgColor);
	CONVERT_IMAGE_TO_DIB(imgGray, dib)

	AfxPrintInfo(_T("[�׷��̽����� ��ȯ] �Է� ����: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateColorGrayscale(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnColorSplitRgb()
{
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor)
	IppByteImage imgR, imgG, imgB;
	IppColorSplitRGB(imgColor, imgR, imgG, imgB);
	CONVERT_IMAGE_TO_DIB(imgR, dibR)
	CONVERT_IMAGE_TO_DIB(imgG, dibG)
	CONVERT_IMAGE_TO_DIB(imgB, dibB)

	AfxPrintInfo(_T("[���� ��� ������/RGB] �Է� ����: %s"), GetTitle());
	AfxNewBitmap(dibR);
	AfxNewBitmap(dibG);
	AfxNewBitmap(dibB);
}


void CImageToolDoc::OnUpdateColorSplitRgb(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnColorSplitHsi()
{
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor)
	IppByteImage imgH, imgS, imgI;
	IppColorSplitHSI(imgColor, imgH, imgS, imgI);
	CONVERT_IMAGE_TO_DIB(imgH, dibH)
	CONVERT_IMAGE_TO_DIB(imgS, dibS)
	CONVERT_IMAGE_TO_DIB(imgI, dibI)

	AfxPrintInfo(_T("[���� ��� ������/HSI] �Է� ����: %s"), GetTitle());
	AfxNewBitmap(dibH);
	AfxNewBitmap(dibS);
	AfxNewBitmap(dibI);
}


void CImageToolDoc::OnUpdateColorSplitHsi(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnColorSplitYuv()
{
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, imgColor)
	IppByteImage imgY, imgU, imgV;
	IppColorSplitYUV(imgColor, imgY, imgU, imgV);
	CONVERT_IMAGE_TO_DIB(imgY, dibY)
	CONVERT_IMAGE_TO_DIB(imgU, dibU)
	CONVERT_IMAGE_TO_DIB(imgV, dibV)

	AfxPrintInfo(_T("[���� ��� ������/YUV] �Է� ����: %s"), GetTitle());
	AfxNewBitmap(dibY);
	AfxNewBitmap(dibU);
	AfxNewBitmap(dibV);
}


void CImageToolDoc::OnUpdateColorSplitYuv(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnColorCombineRgb()
{
	CColorCombineDlg dlg;
	dlg.m_strColorSpace = _T("RGB ���� ��� ��ġ��");
	if (dlg.DoModal() == IDOK)
	{
		CImageToolDoc* pDoc1 = (CImageToolDoc*)dlg.m_pDoc1;
		CImageToolDoc* pDoc2 = (CImageToolDoc*)dlg.m_pDoc2;
		CImageToolDoc* pDoc3 = (CImageToolDoc*)dlg.m_pDoc3;

		CONVERT_DIB_TO_BYTEIMAGE(pDoc1->m_Dib, imgR)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc2->m_Dib, imgG)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc3->m_Dib, imgB)

		IppRgbImage imgColor;
		if (IppColorCombineRGB(imgR, imgG, imgB, imgColor) == false)
		{
			AfxMessageBox(_T("������ ũ�Ⱑ �ٸ��ϴ�!"));
			return;
		}

		CONVERT_IMAGE_TO_DIB(imgColor, dib)

		AfxPrintInfo(_T("[���� ��� ��ġ��/RGB] �Է� ����: R: %s, G: %s, B: %s"), 
			pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnColorCombineHsi()
{
	CColorCombineDlg dlg;
	dlg.m_strColorSpace = _T("HSI ���� ��� ��ġ��");
	if (dlg.DoModal() == IDOK)
	{
		CImageToolDoc* pDoc1 = (CImageToolDoc*)dlg.m_pDoc1;
		CImageToolDoc* pDoc2 = (CImageToolDoc*)dlg.m_pDoc2;
		CImageToolDoc* pDoc3 = (CImageToolDoc*)dlg.m_pDoc3;

		CONVERT_DIB_TO_BYTEIMAGE(pDoc1->m_Dib, imgH)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc2->m_Dib, imgS)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc3->m_Dib, imgI)

		IppRgbImage imgColor;
		if (IppColorCombineHSI(imgH, imgS, imgI, imgColor) == false)
		{
			AfxMessageBox(_T("������ ũ�Ⱑ �ٸ��ϴ�!"));
			return;
		}

		CONVERT_IMAGE_TO_DIB(imgColor, dib)

		AfxPrintInfo(_T("[���� ��� ��ġ��/HSI] �Է� ����: H: %s, S: %s, I: %s"),
			pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnColorCombineYuv()
{
	CColorCombineDlg dlg;
	dlg.m_strColorSpace = _T("YUV ���� ��� ��ġ��");
	if (dlg.DoModal() == IDOK)
	{
		CImageToolDoc* pDoc1 = (CImageToolDoc*)dlg.m_pDoc1;
		CImageToolDoc* pDoc2 = (CImageToolDoc*)dlg.m_pDoc2;
		CImageToolDoc* pDoc3 = (CImageToolDoc*)dlg.m_pDoc3;

		CONVERT_DIB_TO_BYTEIMAGE(pDoc1->m_Dib, imgY)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc2->m_Dib, imgU)
		CONVERT_DIB_TO_BYTEIMAGE(pDoc3->m_Dib, imgV)

		IppRgbImage imgColor;
		if (IppColorCombineYUV(imgY, imgU, imgV, imgColor) == false)
		{
			AfxMessageBox(_T("������ ũ�Ⱑ �ٸ��ϴ�!"));
			return;
		}

		CONVERT_IMAGE_TO_DIB(imgColor, dib)

		AfxPrintInfo(_T("[���� ��� ��ġ��/YUV] �Է� ����: Y: %s, U: %s, V: %s"),
			pDoc1->GetTitle(), pDoc2->GetTitle(), pDoc3->GetTitle());
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnColorEdge()
{
	CONVERT_DIB_TO_RGBIMAGE(m_Dib, img)
	IppByteImage imgEdge;
	IppColorEdge(img, imgEdge);
	CONVERT_IMAGE_TO_DIB(imgEdge, dib)

	AfxPrintInfo(_T("[�÷� ���� ����] �Է� ����: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateColorEdge(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 24);
}


void CImageToolDoc::OnSegmentBinarization()
{
	CBinarizationDlg dlg;
	dlg.SetImage(m_Dib);
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppByteImage imgRes;
		IppBinarization(img, imgRes, dlg.m_nThreshold);
		CONVERT_IMAGE_TO_DIB(imgRes, dib)

		AfxPrintInfo(_T("[����ȭ] �Է� ����: %s, �Ӱ谪: %d"), GetTitle(), dlg.m_nThreshold);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateSegmentBinarization(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnSegmentLabeling()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppIntImage imgLabel;
	std::vector<IppLabelInfo> labels;
	int label_cnt = IppLabeling(img, imgLabel, labels);

	// ��ü�� ���δ� �簢�� �׸���
	BYTE** ptr = img.GetPixels2D();
	for (IppLabelInfo& info : labels)
	{
		for (int j = info.miny; j <= info.maxy; j++)
			ptr[j][info.minx] = ptr[j][info.maxx] = 128;

		for (int i = info.minx; i <= info.maxx; i++)
			ptr[info.miny][i] = ptr[info.maxy][i] = 128;
	}

#if 0
	for (IppLabelInfo& info : labels)
	{
		// ������ ��ü�� �� â���� ����
		IppByteImage imgObj(info.maxx - info.minx + 1, info.maxy - info.miny + 1);
		BYTE** pObj = imgObj.GetPixels2D();
		for (IppPoint& pt : info.pixels)
			pObj[pt.y - info.miny][pt.x - info.minx] = 255;

		CONVERT_IMAGE_TO_DIB(imgObj, dibObj)
		AfxNewBitmap(dibObj);
	}
#endif

	CONVERT_IMAGE_TO_DIB(img, dib)

	AfxPrintInfo(_T("[���̺�] �Է� ����: %s, ��ü ����: %d"), GetTitle(), label_cnt);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateSegmentLabeling(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnContourTracing()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppIntImage imgLabel;
	std::vector<IppLabelInfo> labels;
	int label_cnt = IppLabeling(img, imgLabel, labels);

	IppByteImage imgContour(img.GetWidth(), img.GetHeight());
	BYTE** ptr = imgContour.GetPixels2D();
	for (IppLabelInfo& info : labels)
	{
		std::vector<IppPoint> cp;
		IppContourTracing(img, info.pixels[0].x, info.pixels[0].y, cp);

		for (IppPoint& pt : cp)
			ptr[pt.y][pt.x] = 255;
	}

	CONVERT_IMAGE_TO_DIB(imgContour, dib)

	AfxPrintInfo(_T("[�ܰ��� ����] �Է� ����: %s, ��ü ����: %d"), GetTitle(), label_cnt);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateContourTracing(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnMorphologyErosion()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgDst;
	IppMorphologyErosion(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[���� ��������/ħ��] �Է� ����: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateMorphologyErosion(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnMorphologyDilation()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgDst;
	IppMorphologyDilation(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[���� ��������/��â] �Է� ����: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateMorphologyDilation(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnMorphologyOpening()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgDst;
	IppMorphologyOpening(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[���� ��������/����] �Է� ����: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateMorphologyOpening(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnMorphologyClosing()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgDst;
	IppMorphologyClosing(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[���� ��������/�ݱ�] �Է� ����: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateMorphologyClosing(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnGraymorphErosion()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgDst;
	IppMorphologyGrayErosion(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[�׷��̽����� ��������/ħ��] �Է� ����: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateGraymorphErosion(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnGraymorphDilation()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgDst;
	IppMorphologyGrayDilation(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[�׷��̽����� ��������/�ݱ�] �Է� ����: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateGraymorphDilation(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnGraymorphOpening()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgDst;
	IppMorphologyGrayOpening(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[�׷��̽����� ��������/����] �Է� ����: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateGraymorphOpening(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnGraymorphClosing()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	IppByteImage imgDst;
	IppMorphologyGrayClosing(img, imgDst);
	CONVERT_IMAGE_TO_DIB(imgDst, dib)

	AfxPrintInfo(_T("[�׷��̽����� ��������/�ݱ�] �Է� ����: %s"), GetTitle());
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateGraymorphClosing(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnFourierDescriptor()
{
	CFourierDescDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
		IppIntImage imgLabel;
		std::vector<IppLabelInfo> labels;
		int label_cnt = IppLabeling(img, imgLabel, labels);

		IppByteImage imgContour;
		imgContour.CreateImage(img.GetWidth(), img.GetHeight());
		BYTE** ptr = imgContour.GetPixels2D();

		for (IppLabelInfo& info : labels)
		{
			std::vector<IppPoint> cp;
			IppFourierDescriptor(img, info.pixels[0].x, info.pixels[0].y, dlg.m_nPercent, cp);

			for (IppPoint pt : cp)
			{
				ptr[pt.y][pt.x] = 255;
			}
		}

		CONVERT_IMAGE_TO_DIB(imgContour, dib)

		AfxPrintInfo(_T("[Ǫ���� �����] �Է� ����: %s, ��ü ����: %d, ���� ����: %d%%"), 
			GetTitle(), label_cnt, dlg.m_nPercent);
		AfxNewBitmap(dib);
	}
}


void CImageToolDoc::OnUpdateFourierDescriptor(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnInvariantMoments()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	double m[7] = { 0., };
	IppInvariantMoments(img, m);

	double mm[7] = { 0., };
	CString strMoments = _T("");
	for (int i = 0; i < 7; i++) 
	{
		if (m[i] == 0) continue;
		mm[i] = abs(log(abs(m[i])));
		strMoments.AppendFormat(_T("m[%d] = %6.3lf"), i + 1, mm[i]);
		if (i < 6) strMoments.Append(_T(", "));
	}

	AfxPrintInfo(_T("[�Һ� ���Ʈ] �Է� ����: %s, �Һ� ���Ʈ(log): %s"),
		GetTitle(), strMoments);
}


void CImageToolDoc::OnUpdateInvariantMoments(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnZernikeMoments()
{
	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)

	AfxPrintInfo(_T("[����Ű ���Ʈ] �Է� ����: %s, ����Ű ���Ʈ(Mag) (n <= 6):"), GetTitle());

	double zr, zi;
	CString strMoments = _T("");
	for (int n = 0; n <= 6; n++)
	{
		strMoments = _T("  ");
		for (int m = (n % 2); m <= n; m += 2)
		{
			IppZernikeMoments(img, n, m, zr, zi);
			strMoments.AppendFormat(_T("|zm(%d, %d)| = %7.3lf"), n, m, hypot(zr, zi));
			if (m < n - 1) strMoments.Append(_T(", "));
		}
		AfxPrintInfo(strMoments);
	}
}


void CImageToolDoc::OnUpdateZernikeMoments(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}


void CImageToolDoc::OnTemplateMatching()
{
	TCHAR szFilter[] = _T("Bitmap Files (*.BMP)|*.BMP|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if (dlg.DoModal() != IDOK)
		return;

	CString pathName = dlg.GetPathName();
	IppDib dibTmpl;
	if (!dibTmpl.Load(CT2A(pathName)))
	{
		AfxMessageBox(_T("������ �ҷ����� ���߽��ϴ�."));
		return;
	}
	AfxNewBitmap(dibTmpl);

	if (m_Dib.GetWidth() < dibTmpl.GetWidth() || m_Dib.GetHeight() < dibTmpl.GetHeight())
	{
		AfxMessageBox(_T("���ø� ������ ũ�Ⱑ �Է� ���󺸴� Ů�ϴ�."));
		return;
	}

	CONVERT_DIB_TO_BYTEIMAGE(m_Dib, img)
	CONVERT_DIB_TO_BYTEIMAGE(dibTmpl, imgTmpl)
	IppIntImage imgMap;
	IppPoint dp = IppTemplateMatching(img, imgTmpl, imgMap);

#if 0
	{
		IppByteImage imgCvt(img.GetWidth(), img.GetHeight());
		BYTE* pCvt = imgCvt.GetPixels();
		int* pMap = imgMap.GetPixels();

		int max_value = 0;
		for (int i = 0; i < img.GetSize(); i++)
		{
			if (pMap[i] > max_value) max_value = pMap[i];
		}

		if (max_value != 0)
		{
			for (int i = 0; i < img.GetSize(); i++)
			{
				pCvt[i] = pMap[i] * 255 / max_value;
			}
		}

		CONVERT_IMAGE_TO_DIB(imgCvt, dibMap)
		AfxNewBitmap(dibMap);
	}
#endif

	{ // �Է� ���� BOX �׸���
		int tw2 = imgTmpl.GetWidth() / 2;
		int th2 = imgTmpl.GetHeight() / 2;

		int minx = dp.x - tw2;
		int maxx = dp.x + tw2;
		int miny = dp.y - th2;
		int maxy = dp.y + th2;

		BYTE** ptr = img.GetPixels2D();

		for (int j = miny; j < maxy; j++)
			ptr[j][minx] = ptr[j][maxx] = 255;

		for (int i = minx; i < maxx; i++)
			ptr[miny][i] = ptr[maxy][i] = 255;
	}

	CONVERT_IMAGE_TO_DIB(img, dib)

	AfxPrintInfo(_T("[���ø� ��Ī] �Է� ����: %s, ���ø� ����: %s, ���� ��ǥ: (%d, %d)"),
		GetTitle(), dlg.GetFileName(), dp.x, dp.y);
	AfxNewBitmap(dib);
}


void CImageToolDoc::OnUpdateTemplateMatching(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Dib.GetBitCount() == 8);
}
