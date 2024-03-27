
// MyImageProcessView.cpp: CMyImageProcessView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MyImageProcess.h"
#endif

#include "MyImageProcessDoc.h"
#include "MyImageProcessView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyImageProcessView

IMPLEMENT_DYNCREATE(CMyImageProcessView, CView)

BEGIN_MESSAGE_MAP(CMyImageProcessView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_EqualImage, &CMyImageProcessView::OnEqualimage)
	ON_COMMAND(ID_AddImage, &CMyImageProcessView::OnAddimage)
	ON_COMMAND(ID_InverseImage, &CMyImageProcessView::OnInverseImage)
	ON_COMMAND(ID_GammaImage, &CMyImageProcessView::OnGammaImage)
	ON_COMMAND(ID_HistStretch, &CMyImageProcessView::OnHistStretch)
	ON_COMMAND(ID_HistEqual, &CMyImageProcessView::OnHistequal)
	ON_COMMAND(ID_RotateImage, &CMyImageProcessView::OnRotateImage)
	ON_COMMAND(ID_EmbossImage, &CMyImageProcessView::OnEmbossImage)
	ON_COMMAND(ID_AverageBlur, &CMyImageProcessView::OnAverageBlur)
	ON_COMMAND(ID_PrewittYEdge, &CMyImageProcessView::OnPrewittYEdge)
	ON_COMMAND(ID_PrewittXEdge, &CMyImageProcessView::OnPrewittXEdge)
	ON_COMMAND(ID_SobelXEdge, &CMyImageProcessView::OnSobelXEdge)
	ON_COMMAND(ID_SobelYEdge, &CMyImageProcessView::OnSobelYEdge)
	ON_COMMAND(ID_ZoomIn, &CMyImageProcessView::OnZoomIn)
	ON_COMMAND(ID_ZoomOut, &CMyImageProcessView::OnZoomOut)
	ON_COMMAND(ID_Laplacian1DEdge, &CMyImageProcessView::OnLaplacian1DEdge)
	ON_COMMAND(ID_Laplacian2DEdge, &CMyImageProcessView::OnLaplacian2Dedge)
	ON_COMMAND(ID_BinByVal, &CMyImageProcessView::OnBinByVal)
	ON_COMMAND(ID_BinByAvg, &CMyImageProcessView::OnBinByAvg)
	ON_COMMAND(ID_BinByMedian, &CMyImageProcessView::OnBinByMedian)
	ON_COMMAND(ID_GaussianBlur, &CMyImageProcessView::OnGaussianblur)
END_MESSAGE_MAP()

// CMyImageProcessView 생성/소멸

CMyImageProcessView::CMyImageProcessView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMyImageProcessView::~CMyImageProcessView()
{
}

BOOL CMyImageProcessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMyImageProcessView 그리기

void CMyImageProcessView::OnDraw(CDC* pDC)
{
	CMyImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	/*unsigned char R, G, B;
	for (int i = 0; i < pDoc->m_inH; i++) {
		for (int k = 0; k < pDoc->m_inW; k++) {
			R = G = B = pDoc->m_inImage[i][k];
			pDC->SetPixel(k + 5, i + 5, RGB(R, G, B));
		}
	}
	for (int i = 0; i < pDoc->m_outH; i++) {
		for (int k = 0; k < pDoc->m_outW; k++) {
			R = G = B = pDoc->m_outImage[i][k];
			pDC->SetPixel(k + pDoc->m_inW + 10, i + 5, RGB(R, G, B));
		}
	}*/


	/////////////////////
	/// 성능 개선을 위한 더블 버퍼링 
	////////////////////
	int i, k;
	unsigned char R, G, B;
	// 메모리 DC 선언
	CDC memDC;
	CBitmap* pOldBitmap, bitmap;

	// 화면 DC와 호환되는 메모리 DC 객체를 생성
	memDC.CreateCompatibleDC(pDC);

	// 마찬가지로 화면 DC와 호환되는 Bitmap 생성
	bitmap.CreateCompatibleBitmap(pDC, pDoc->m_inW, pDoc->m_inH);

	pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, pDoc->m_inW, pDoc->m_inH, WHITENESS); // 흰색으로 초기화

	// 메모리 DC에 그리기
	for (i = 0; i < pDoc->m_inH; i++) {
		for (k = 0; k < pDoc->m_inW; k++) {
			R = G = B = pDoc->m_inImage[i][k];
			memDC.SetPixel(k, i, RGB(R, G, B));
		}
	}
	// 메모리 DC를 화면 DC에 고속 복사
	pDC->BitBlt(5, 5, pDoc->m_inW, pDoc->m_inH, &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();

	///////////////////

	// 화면 DC와 호환되는 메모리 DC 객체를 생성
	memDC.CreateCompatibleDC(pDC);

	// 마찬가지로 화면 DC와 호환되는 Bitmap 생성
	bitmap.CreateCompatibleBitmap(pDC, pDoc->m_outW, pDoc->m_outH);

	pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, pDoc->m_outW, pDoc->m_outH, WHITENESS); // 흰색으로 초기화

	// 메모리 DC에 그리기
	for (i = 0; i < pDoc->m_outH; i++) {
		for (k = 0; k < pDoc->m_outW; k++) {
			R = G = B = pDoc->m_outImage[i][k];
			memDC.SetPixel(k, i, RGB(R, G, B));
		}
	}
	// 메모리 DC를 화면 DC에 고속 복사
	pDC->BitBlt(pDoc->m_inW + 10, 5, pDoc->m_outW, pDoc->m_outH, &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}


// CMyImageProcessView 인쇄

BOOL CMyImageProcessView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMyImageProcessView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMyImageProcessView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMyImageProcessView 진단

#ifdef _DEBUG
void CMyImageProcessView::AssertValid() const
{
	CView::AssertValid();
}

void CMyImageProcessView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyImageProcessDoc* CMyImageProcessView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyImageProcessDoc)));
	return (CMyImageProcessDoc*)m_pDocument;
}
#endif //_DEBUG


// CMyImageProcessView 메시지 처리기


void CMyImageProcessView::OnEqualimage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMyImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEqualImage();
	Invalidate(TRUE);

}


//오타있음 나중에 고칠람 고치셈
void CMyImageProcessView::OnAddimage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMyImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnAddimage();
	Invalidate(TRUE);
}


void CMyImageProcessView::OnInverseImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMyImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnInverseImage();
	Invalidate(TRUE);
}


void CMyImageProcessView::OnGammaImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMyImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnGammaImage();
	Invalidate(TRUE);
}


void CMyImageProcessView::OnHistStretch()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMyImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->HistStretch();
	Invalidate(TRUE);
}


void CMyImageProcessView::OnHistequal()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMyImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnHistEqual();
	Invalidate(TRUE);
}


void CMyImageProcessView::OnRotateImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMyImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnRotateImage();
	Invalidate(TRUE);
}


void CMyImageProcessView::OnEmbossImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMyImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEmbossImage();
	Invalidate(TRUE);
}


void CMyImageProcessView::OnAverageBlur()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMyImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnAverageBlur();
	Invalidate(TRUE);
}

void CMyImageProcessView::OnPrewittXEdge()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMyImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnPrewittXEdge();
	Invalidate(TRUE);
}


void CMyImageProcessView::OnPrewittYEdge()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMyImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnPrewittYEdge();
	Invalidate(TRUE);
}



void CMyImageProcessView::OnSobelXEdge()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMyImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->SobelXEdge();
	Invalidate(TRUE);
}


void CMyImageProcessView::OnSobelYEdge()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMyImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnSobelYEdge();
	Invalidate(TRUE);
}


void CMyImageProcessView::OnZoomIn()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMyImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int temp = pDoc->OnZoomIn();
	Invalidate(TRUE);
}


void CMyImageProcessView::OnZoomOut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMyImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int temp = pDoc->OnZoomOut();
	Invalidate(TRUE);
}


void CMyImageProcessView::OnLaplacian1DEdge()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMyImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnLaplacian1DEdge();
	Invalidate(TRUE);
}


void CMyImageProcessView::OnLaplacian2Dedge()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMyImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnLaplacian2DEdge();
	Invalidate(TRUE);
}


void CMyImageProcessView::OnBinByVal()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMyImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnBinByVal();
	Invalidate(TRUE);
}


void CMyImageProcessView::OnBinByAvg()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMyImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnBinByAvg();
	Invalidate(TRUE);
}


void CMyImageProcessView::OnBinByMedian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMyImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnBinByMedian();
	Invalidate(TRUE);
}


void CMyImageProcessView::OnGaussianblur()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMyImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnGaussianBlur();
	Invalidate(TRUE);
}
