
// ColorImageProcessDoc.cpp: CColorImageProcessDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ColorImageProcess.h"
#endif

#include "ColorImageProcessDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CColorImageProcessDoc

IMPLEMENT_DYNCREATE(CColorImageProcessDoc, CDocument)

BEGIN_MESSAGE_MAP(CColorImageProcessDoc, CDocument)
END_MESSAGE_MAP()


// CColorImageProcessDoc 생성/소멸

CColorImageProcessDoc::CColorImageProcessDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CColorImageProcessDoc::~CColorImageProcessDoc()
{
}

BOOL CColorImageProcessDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CColorImageProcessDoc serialization

void CColorImageProcessDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CColorImageProcessDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
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

// 검색 처리기를 지원합니다.
void CColorImageProcessDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CColorImageProcessDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CColorImageProcessDoc 진단

#ifdef _DEBUG
void CColorImageProcessDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CColorImageProcessDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CColorImageProcessDoc 명령


BOOL CColorImageProcessDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// 기존 메모리 해제
	if (m_inImageR != NULL) {
		OnFree2D(m_inImageR, m_inH);
		OnFree2D(m_inImageG, m_inH);
		OnFree2D(m_inImageB, m_inH);
		m_inImageR = m_inImageG = m_inImageB = NULL;
		m_inH = m_inW = 0;

		OnFree2D(m_outImageR, m_outH);
		OnFree2D(m_outImageG, m_outH);
		OnFree2D(m_outImageB, m_outH);
		m_outImageR = m_outImageG = m_outImageB = NULL;
		m_outH = m_outW = 0;
	}

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CImage image;
	image.Load(lpszPathName);
	// 입력 영상 크기 알아내기
	m_inH = image.GetHeight();
	m_inW = image.GetWidth();
	// 메모리 할당
	m_inImageR = OnMalloc2D(m_inH, m_inW);
	m_inImageG = OnMalloc2D(m_inH, m_inW);
	m_inImageB = OnMalloc2D(m_inH, m_inW);
	// CImage의 객체값 --> 메모리
	COLORREF  px;
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			px = image.GetPixel(k, i);
			m_inImageR[i][k] = GetRValue(px);
			m_inImageG[i][k] = GetGValue(px);
			m_inImageB[i][k] = GetBValue(px);
		}

	return TRUE;
}

void CColorImageProcessDoc::OnCloseDocument()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	OnFree2D(m_inImageR, m_inH);
	OnFree2D(m_inImageG, m_inH);
	OnFree2D(m_inImageB, m_inH);

	OnFree2D(m_outImageR, m_outH);
	OnFree2D(m_outImageG, m_outH);
	OnFree2D(m_outImageB, m_outH);

	CDocument::OnCloseDocument();
}


BOOL CColorImageProcessDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (m_outImageR == NULL)
		return FALSE;
	CImage image;
	image.Create(m_outW, m_outH, 32);
	unsigned char R, G, B;
	COLORREF px;
	for (int i = 0; i < m_outW; i++)
		for (int k = 0; k < m_outH; k++) {
			R = m_outImageR[k][i];
			G = m_outImageG[k][i];
			B = m_outImageB[k][i];
			px = RGB(R, G, B);
			image.SetPixel(i, k, px);
		}
	image.Save(lpszPathName, Gdiplus::ImageFormatPNG);
	MessageBox(NULL, L"저장", L"성공", NULL);
	return TRUE;
}


unsigned char** CColorImageProcessDoc::OnMalloc2D(int h, int w)
{
	// TODO: 여기에 구현 코드 추가.
	unsigned char** memory;
	memory = new unsigned char* [h];
	for (int i = 0; i < h; i++)
		memory[i] = new unsigned char[w];
	return memory;
}


void CColorImageProcessDoc::OnFree2D(unsigned char** memory, int h)
{
	// TODO: 여기에 구현 코드 추가.
	if (memory == NULL)
		return;
	for (int i = 0; i < h; i++)
		delete memory[i];
	delete[] memory;
}

void CColorImageProcessDoc::OnFreeOutImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_outImageR != NULL) {
		OnFree2D(m_outImageR, m_outH);
		OnFree2D(m_outImageG, m_outH);
		OnFree2D(m_outImageB, m_outH);
		m_outImageR = m_outImageG = m_outImageB = NULL;
		m_outH = m_outW = 0;
	}
}

void CColorImageProcessDoc::OnMallocOutImage()
{
	// TODO: 여기에 구현 코드 추가.
	m_outImageR = OnMalloc2D(m_outH, m_outW);
	m_outImageG = OnMalloc2D(m_outH, m_outW);
	m_outImageB = OnMalloc2D(m_outH, m_outW);
}


void CColorImageProcessDoc::OnEqualmage()
{
	// TODO: 여기에 구현 코드 추가.
	// 기존 메모리 해제
	OnFreeOutImage();

	// 메모리 할당
	m_outH = m_inH;
	m_outW = m_inW;
	OnMallocOutImage();

	// ** 진짜 영상처리 알고리즘 **
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageR[i][k] = m_inImageR[i][k];
			m_outImageG[i][k] = m_inImageG[i][k];
			m_outImageB[i][k] = m_inImageB[i][k];
		}
	}
}


void CColorImageProcessDoc::OnGrayScale()
{
	// TODO: 여기에 구현 코드 추가.
	// 기존 메모리 해제
	OnFreeOutImage();

	// 메모리 할당
	m_outH = m_inH;
	m_outW = m_inW;
	OnMallocOutImage();

	// ** 진짜 영상처리 알고리즘 **
	double avg;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
			m_outImageR[i][k] = m_outImageG[i][k] = m_outImageB[i][k] = (unsigned char)avg;
		}
	}
}

