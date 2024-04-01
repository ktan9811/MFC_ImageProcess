
// ColorImageProcessView.cpp: CColorImageProcessView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ColorImageProcess.h"
#endif

#include "ColorImageProcessDoc.h"
#include "ColorImageProcessView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CColorImageProcessView

IMPLEMENT_DYNCREATE(CColorImageProcessView, CView)

BEGIN_MESSAGE_MAP(CColorImageProcessView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_EqualImage, &CColorImageProcessView::OnEqualmage)
	ON_COMMAND(ID_GrayScale, &CColorImageProcessView::OnGrayScale)
END_MESSAGE_MAP()

// CColorImageProcessView 생성/소멸

CColorImageProcessView::CColorImageProcessView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CColorImageProcessView::~CColorImageProcessView()
{
}

BOOL CColorImageProcessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CColorImageProcessView 그리기

void CColorImageProcessView::OnDraw(CDC* pDC)
{
	//CColorImageProcessDoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);
	//if (!pDoc)
	//	return;

	//// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	//int R, G, B;
	//for (int i = 0; i < pDoc->m_inH; i++)
	//	for (int k = 0; k < pDoc->m_inW; k++) {
	//		R = pDoc->m_inImageR[i][k];
	//		G = pDoc->m_inImageG[i][k];
	//		B = pDoc->m_inImageB[i][k];
	//		pDC->SetPixel(k + 5, i + 5, RGB(R, G, B));
	//	}

	//for (int i = 0; i < pDoc->m_outH; i++)
	//	for (int k = 0; k < pDoc->m_outW; k++) {
	//		R = pDoc->m_outImageR[i][k];
	//		G = pDoc->m_outImageG[i][k];
	//		B = pDoc->m_outImageB[i][k];
	//		pDC->SetPixel(pDoc->m_inW + k + 5 + 5, i + 5, RGB(R, G, B));
	//	}

	///////////////////
	/// ** 더블 버퍼링 **
	///////////////////
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

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
			R = pDoc->m_inImageR[i][k];
			G = pDoc->m_inImageG[i][k];
			B = pDoc->m_inImageB[i][k];
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
			R = pDoc->m_outImageR[i][k];
			G = pDoc->m_outImageG[i][k];
			B = pDoc->m_outImageB[i][k];
			memDC.SetPixel(k, i, RGB(R, G, B));
		}
	}
	// 메모리 DC를 화면 DC에 고속 복사
	pDC->BitBlt(pDoc->m_inW + 10, 5, pDoc->m_outW, pDoc->m_outH, &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}



// CColorImageProcessView 인쇄

BOOL CColorImageProcessView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CColorImageProcessView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CColorImageProcessView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CColorImageProcessView 진단

#ifdef _DEBUG
void CColorImageProcessView::AssertValid() const
{
	CView::AssertValid();
}

void CColorImageProcessView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CColorImageProcessDoc* CColorImageProcessView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CColorImageProcessDoc)));
	return (CColorImageProcessDoc*)m_pDocument;
}
#endif //_DEBUG


// CColorImageProcessView 메시지 처리기


void CColorImageProcessView::OnEqualmage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEqualmage();
	Invalidate(TRUE);
}


void CColorImageProcessView::OnGrayScale()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnGrayScale();
	Invalidate(TRUE);
}
