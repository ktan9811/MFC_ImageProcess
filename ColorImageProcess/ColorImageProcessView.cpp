
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
	ON_COMMAND(ID_Change_Hue, &CColorImageProcessView::OnChangeHue)
	ON_COMMAND(ID_Change_Saturation, &CColorImageProcessView::OnChangeSaturation)
	ON_COMMAND(ID_Change_Intensity, &CColorImageProcessView::OnChangeIntensity)
	ON_COMMAND(ID_Add_Image, &CColorImageProcessView::OnAddImage)
	ON_COMMAND(ID_Inverse_Image, &CColorImageProcessView::OnInverseImage)
	ON_COMMAND(ID_Image_Gamma, &CColorImageProcessView::OnImageGamma)
	ON_COMMAND(ID_Rotate_By_Degree, &CColorImageProcessView::OnRotateByDegree)
	ON_COMMAND(ID_Zoom_In, &CColorImageProcessView::OnZoomIn)
	ON_COMMAND(ID_Zoom_Out, &CColorImageProcessView::OnZoomOut)
	ON_COMMAND(ID_CyberPunk, &CColorImageProcessView::OnCyberpunk)
	ON_COMMAND(ID_Avg_Blur, &CColorImageProcessView::OnAvgBlur)
	ON_COMMAND(ID_GaussianBlur, &CColorImageProcessView::OnGaussianblur)
	ON_COMMAND(ID_Emboss_Image, &CColorImageProcessView::OnEmbossImage)
	ON_COMMAND(ID_PREWITT_XEDGE, &CColorImageProcessView::OnPrewittXedge)
	ON_COMMAND(ID_PREWITT_YEDGE, &CColorImageProcessView::OnPrewittYedge)
	ON_COMMAND(ID_SOBLE_YEDGE, &CColorImageProcessView::OnSobleYedge)
	ON_COMMAND(ID_SOBLE_XEDGE, &CColorImageProcessView::OnSobleXedge)
	ON_COMMAND(ID_Laplacian_1D_Edge, &CColorImageProcessView::OnLaplacian1dEdge)
	ON_COMMAND(ID_Laplacian_2D_Edge, &CColorImageProcessView::OnLaplacian2dEdge)
	ON_COMMAND(ID_Laplacian_Of_Gaussian, &CColorImageProcessView::OnLaplacianOfGaussian)
	ON_COMMAND(ID_Hist_Strech, &CColorImageProcessView::OnHistStrech)
	ON_COMMAND(ID_Hist_Equal, &CColorImageProcessView::OnHistEqual)
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
	///////////////////
	/// ** 더블 버퍼링 **
	///////////////////
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

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


	// 출력 영상의 크기를 자동 조절
	double MAXSIZE = 800;  // 필요시 실 모니터 또는 화면의 해상도에 따라서 변경 가능!
	int inH = pDoc->m_inH;
	int inW = pDoc->m_inW;
	double hop = 1.0; // 기본

	if (inH > MAXSIZE || inW > MAXSIZE) {
		// hop을 새로 계산.
		if (inW > inH)
			hop = (inW / MAXSIZE);
		else
			hop = (inH / MAXSIZE);

		inW = (int)(inW / hop);
		inH = (int)(inH / hop);
	}

	// 메모리 DC에 그리기
	for (i = 0; i < inH; i++) {
		for (k = 0; k < inW; k++) {
			R = pDoc->m_inImageR[(int)(i * hop)][(int)(k * hop)];
			G = pDoc->m_inImageG[(int)(i * hop)][(int)(k * hop)];
			B = pDoc->m_inImageB[(int)(i * hop)][(int)(k * hop)];
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

	int outH = pDoc->m_outH;
	int outW = pDoc->m_outW;
	hop = 1.0; // 기본

	if (outH > MAXSIZE || outW > MAXSIZE) {
		// hop을 새로 계산.
		if (outW > outH)
			hop = (outW / MAXSIZE);
		else
			hop = (outH / MAXSIZE);

		outW = (int)(outW / hop);
		outH = (int)(outH / hop);
	}

	// 메모리 DC에 그리기
	for (i = 0; i < outH; i++) {
		for (k = 0; k < outW; k++) {
			R = pDoc->m_outImageR[(int)(i * hop)][(int)(k * hop)];
			G = pDoc->m_outImageG[(int)(i * hop)][(int)(k * hop)];
			B = pDoc->m_outImageB[(int)(i * hop)][(int)(k * hop)];
			memDC.SetPixel(k, i, RGB(R, G, B));
		}
	}
	// 메모리 DC를 화면 DC에 고속 복사
	pDC->BitBlt(inW + 20, 5, pDoc->m_outW, pDoc->m_outH, &memDC, 0, 0, SRCCOPY);

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

void CColorImageProcessView::OnAddImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnAddImage();
	Invalidate(TRUE);
}



void CColorImageProcessView::OnChangeHue()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnChangeHue();
	Invalidate(TRUE);
}


void CColorImageProcessView::OnChangeSaturation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnChangeSaturation();
	Invalidate(TRUE);
}


void CColorImageProcessView::OnChangeIntensity()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int temp = pDoc->OnChangeIntensity();
	Invalidate(TRUE);
}


void CColorImageProcessView::OnInverseImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnInverseImage();
	Invalidate(TRUE);
}


void CColorImageProcessView::OnImageGamma()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnImageGamma();
	Invalidate(TRUE);
}


void CColorImageProcessView::OnRotateByDegree()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnRotateByDegree();
	Invalidate(TRUE);
}


void CColorImageProcessView::OnZoomIn()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnZoomIn();
	Invalidate(TRUE);
}


void CColorImageProcessView::OnZoomOut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnZoomOut();
	Invalidate(TRUE);
}


void CColorImageProcessView::OnCyberpunk()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnCyberpunk();
	Invalidate(TRUE);
}


void CColorImageProcessView::OnAvgBlur()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnAvgBlur();
	Invalidate(TRUE);
}


void CColorImageProcessView::OnGaussianblur()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnGaussianBlur();
	Invalidate(TRUE);
}


void CColorImageProcessView::OnEmbossImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEmbossImage();
	Invalidate(TRUE);
}


void CColorImageProcessView::OnPrewittXedge()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnPrewittXedge();
	Invalidate(TRUE);
}


void CColorImageProcessView::OnPrewittYedge()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnPrewittYedge();
	Invalidate(TRUE);
}

void CColorImageProcessView::OnSobleXedge()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int tmep = pDoc->OnSobleXedge();
	Invalidate(TRUE);
}


void CColorImageProcessView::OnSobleYedge()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnSobleYedge();
	Invalidate(TRUE);
}


void CColorImageProcessView::OnLaplacian1dEdge()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnLaplacian1dEdge();
	Invalidate(TRUE);
}


void CColorImageProcessView::OnLaplacian2dEdge()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnLaplacian2dEdge();
	Invalidate(TRUE);
}


void CColorImageProcessView::OnLaplacianOfGaussian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnLaplacianOfGaussian();
	Invalidate(TRUE);
}


void CColorImageProcessView::OnHistStrech()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnHistStrech();
	Invalidate(TRUE);
}


void CColorImageProcessView::OnHistEqual()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnHistEqual();
	Invalidate(TRUE);
}
