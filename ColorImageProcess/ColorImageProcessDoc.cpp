﻿
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

#include "CConstantDialog.h"
#include "CGetString.h"

#define _USE_MATH_DEFINES
#include <math.h>

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

void CColorImageProcessDoc::OnFreeTempImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_outImageR != NULL) {
		OnFree2D(m_tempImageR, m_tempH);
		OnFree2D(m_tempImageG, m_tempH);
		OnFree2D(m_tempImageB, m_tempH);
		m_tempImageR = m_tempImageG = m_tempImageB = NULL;
		m_tempH = m_tempW = 0;
	}
}

void CColorImageProcessDoc::OnMallocTempImage()
{
	// TODO: 여기에 구현 코드 추가.
	m_tempImageR = OnMalloc2D(m_tempH, m_tempW);
	m_tempImageG = OnMalloc2D(m_tempH, m_tempW);
	m_tempImageB = OnMalloc2D(m_tempH, m_tempW);
}

void CColorImageProcessDoc::OnFreeMask()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_mask == NULL)
		return;
	for (int i = 0; i < m_ksize; i++)
		delete m_mask[i];
	delete[] m_mask;
}

double** CColorImageProcessDoc::OnMalloc2DMask(int ksize)
{
	// TODO: 여기에 구현 코드 추가.
	// TODO: 여기에 구현 코드 추가.
	double** memory;
	memory = new double* [ksize];
	for (int i = 0; i < ksize; i++)
		memory[i] = new double[ksize];
	return memory;
}

void CColorImageProcessDoc::OnMallocDoubleImage()
{
	// TODO: 여기에 구현 코드 추가.
	m_doubleImageR = new double* [m_dimH];
	m_doubleImageG = new double* [m_dimH];
	m_doubleImageB = new double* [m_dimH];
	for (int i = 0; i < m_dimH; i++) {
		m_doubleImageR[i] = new double[m_dimW];
		m_doubleImageG[i] = new double[m_dimW];
		m_doubleImageB[i] = new double[m_dimW];
	}
}


void CColorImageProcessDoc::OnFreeDoubleImage()
{
	// TODO: 여기에 구현 코드 추가.
	if (m_doubleImageR == NULL) return;
	if (m_doubleImageG == NULL) return;
	if (m_doubleImageB == NULL) return;

	for (int i = 0; i < m_dimH; i++) {
		delete m_doubleImageR[i];
		delete m_doubleImageG[i];
		delete m_doubleImageB[i];
	}
	delete[] m_doubleImageR;
	delete[] m_doubleImageG;
	delete[] m_doubleImageB;
}

//for (int i = 0; i < h; i++)
//	delete memory[i];
//delete[] memory;


void CColorImageProcessDoc::OnEqualmage()
{
	// TODO: 여기에 구현 코드 추가.
	// 기존 메모리 해제
	OnFreeOutImage();

	// 메모리 할당
	m_outH = m_inH;
	m_outW = m_inW;
	OnMallocOutImage();

	// 영상처리 알고리즘
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

	// 영상처리 알고리즘
	double avg;
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			avg = (m_inImageR[i][k] + m_inImageG[i][k] + m_inImageB[i][k]) / 3.0;
			m_outImageR[i][k] = m_outImageG[i][k] = m_outImageB[i][k] = (unsigned char)avg;
		}
	}
}



double* CColorImageProcessDoc::RGB2HSI(int R, int G, int B)
{
	// TODO: 여기에 구현 코드 추가.
	double H, S, I;
	double* HSI = new double[3];
	double min_value, angle;
	I = (R + G + B) / 3.0; // 밝기
	if ((R == G) && (G == B)) { // 그레이
		S = 0.0;
		H = 0.0;
	}
	else {

		min_value = min(min(R, G), B); //최소값 추출
		angle = (R - 0.5 * G - 0.5 * B) / (double)sqrt((R - G) * (R - G) + (R - B) * (G - B));

		H = (double)acos(angle) * 57.29577951;
		S = 1.0f - (3.0 / (R + G + B)) * min_value;
	}
	if (B > G) H = 360. - H;

	HSI[0] = H;
	HSI[1] = S;
	HSI[2] = I;

	return HSI;
}


unsigned char* CColorImageProcessDoc::HSI2RGB(double H, double S, double I)
{
	// TODO: 여기에 구현 코드 추가.
	double R, G, B;
	unsigned char* RGB = new unsigned char[3];

	double angle1, angle2, scale;

	if (I == 0.0) { // Black
		RGB[0] = 0;
		RGB[1] = 0;
		RGB[2] = 0;
		return RGB;
	}

	if (H <= 0.0) H += 360.0f;

	scale = 3.0 * I;
	if (H <= 120.0)
	{
		angle1 = H * 0.017453293;
		angle2 = (60.0 - H) * 0.017453293;
		B = (1.0 - S) / 3.0f;
		R = (double)(1.0 + (S * cos(angle1) / cos(angle2))) / 3.0;
		G = 1.0 - R - B;
		B *= scale;
		R *= scale;
		G *= scale;
	}


	else if ((H > 120.0) && (H <= 240.0)) {
		H -= 120.0;
		angle1 = H * 0.017453293;

		angle2 = (60.0 - H) * 0.017453293;
		R = (1.0 - S) / 3.0;
		G = (double)(1.0f + (S * cos(angle1) / cos(angle2))) / 3.0;
		B = 1.0 - R - G;
		R *= scale;
		G *= scale;
		B *= scale;
	}
	else {
		H -= 240.0;
		angle1 = H * 0.017453293;
		angle2 = (60.0 - H) * 0.017453293;
		G = (1.0f - S) / 3.0;
		B = (double)(1.0 + (S * cos(angle1) / cos(angle2))) / 3.0;
		R = 1.0 - G - B;

		R *= scale;
		G *= scale;
		B *= scale;
	}

	RGB[0] = (unsigned char)R;
	RGB[1] = (unsigned char)G;
	RGB[2] = (unsigned char)B;
	return RGB;
}


void CColorImageProcessDoc::OnChangeSaturation()
{
	// TODO: 여기에 구현 코드 추가.
	// 기존 메모리 해제
	CConstantDialog dlg;
	if (dlg.DoModal() != IDOK)	return;
	double val = dlg.m_constant;

	OnFreeOutImage();

	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당
	OnMallocOutImage();
	// ** 진짜 영상처리 알고리즘 **
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// HIS 모델 값
			// H(색상) : 0~360
			// S(채도) : 0.0 ~ 1.0
			// I(명도) : 0 ~ 255

			// RGB --> HSI
			double H, S, I;
			unsigned char R, G, B;
			R = m_inImageR[i][k];
			G = m_inImageG[i][k];
			B = m_inImageB[i][k];

			double* hsi = RGB2HSI(R, G, B);
			H = hsi[0];	S = hsi[1];	I = hsi[2];

			/// 채도 조정
			S = S + val;
			if (S < 0) S = 0.0;
			if (S > 1.0) S = 1.0;
			//else if (S > 0.99) S = 0.99;

			// HSI --> RGB
			unsigned char* rgb = HSI2RGB(H, S, I);
			R = rgb[0]; G = rgb[1]; B = rgb[2];

			m_outImageR[i][k] = R;
			m_outImageG[i][k] = G;
			m_outImageB[i][k] = B;
		}
	}
}

int CColorImageProcessDoc::OnChangeIntensity()
{
	// TODO: 여기에 구현 코드 추가.
	// 기존 메모리 해제
	CConstantDialog dlg;
	if (dlg.DoModal() != IDOK)	return 0;
	double val = dlg.m_constant;

	OnFreeOutImage();

	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당
	OnMallocOutImage();
	// ** 진짜 영상처리 알고리즘 **
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// HIS 모델 값
			// H(색상) : 0~360
			// S(채도) : 0.0 ~ 1.0
			// I(명도) : 0 ~ 255

			// RGB --> HSI
			double H, S, I;
			unsigned char R, G, B;
			R = m_inImageR[i][k];
			G = m_inImageG[i][k];
			B = m_inImageB[i][k];

			double* hsi = RGB2HSI(R, G, B);
			H = hsi[0];	S = hsi[1];	I = hsi[2];

			/// I 조정
			I = I + val;
			if (I < 0) I = 0.0;
			if (I > 255) I = 255.0;

			// HSI --> RGB
			unsigned char* rgb = HSI2RGB(H, S, I);
			R = rgb[0]; G = rgb[1]; B = rgb[2];

			m_outImageR[i][k] = R;
			m_outImageG[i][k] = G;
			m_outImageB[i][k] = B;
		}
	}
	return 0;
}


void CColorImageProcessDoc::OnChangeHue()
{
	// TODO: 여기에 구현 코드 추가.
	// 기존 메모리 해제
	CConstantDialog dlg;
	if (dlg.DoModal() != IDOK)	return;
	int val = (int)dlg.m_constant;

	OnFreeOutImage();

	m_outH = m_inH;
	m_outW = m_inW;
	// 메모리 할당
	OnMallocOutImage();
	// ** 진짜 영상처리 알고리즘 **
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			// HIS 모델 값
			// H(색상) : 0~360
			// S(채도) : 0.0 ~ 1.0
			// I(명도) : 0 ~ 255

			// RGB --> HSI
			double H, S, I;
			unsigned char R, G, B;
			R = m_inImageR[i][k];
			G = m_inImageG[i][k];
			B = m_inImageB[i][k];

			double* hsi = RGB2HSI(R, G, B);
			H = hsi[0];	S = hsi[1];	I = hsi[2];

			/// H 조정
			H = H + val;
			H = (int)H % 360;
			if (H < 0.0) H += 360;

			// HSI --> RGB
			unsigned char* rgb = HSI2RGB(H, S, I);
			R = rgb[0]; G = rgb[1]; B = rgb[2];

			m_outImageR[i][k] = R;
			m_outImageG[i][k] = G;
			m_outImageB[i][k] = B;
		}
	}
}


void CColorImageProcessDoc::OnAddImage()
{
	// TODO: 여기에 구현 코드 추가.
	
	// 입력값 받기
	CConstantDialog dlg;
	if (dlg.DoModal() != IDOK)	return;
	int val = int(dlg.m_constant);
	

	// 기존 메모리 해제
	OnFreeOutImage();

	// 메모리 할당
	m_outH = m_inH;
	m_outW = m_inW;
	OnMallocOutImage();

	// 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (m_inImageR[i][k] + val > 255) m_outImageR[i][k] = 255;
			else if (m_inImageR[i][k] + val < 0) m_outImageR[i][k] = 0;
			else m_outImageR[i][k] = m_inImageR[i][k] + val;

			if (m_inImageG[i][k] + val > 255) m_outImageG[i][k] = 255;
			else if (m_inImageG[i][k] + val < 0) m_outImageG[i][k] = 0;
			else m_outImageG[i][k] = m_inImageG[i][k] + val;

			if (m_inImageB[i][k] + val > 255) m_outImageB[i][k] = 255;
			else if (m_inImageB[i][k] + val < 0) m_outImageB[i][k] = 0;
			else m_outImageB[i][k] = m_inImageB[i][k] + val;
		}
	}
}


void CColorImageProcessDoc::OnInverseImage()
{
	// TODO: 여기에 구현 코드 추가.
	// 메모리 헤제
	OnFreeOutImage();

	// 메모리 할당
	m_outH = m_inH;
	m_outW = m_inW;
	OnMallocOutImage();

	// 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageR[i][k] = 255 - m_inImageR[i][k];
			m_outImageG[i][k] = 255 - m_inImageG[i][k];
			m_outImageB[i][k] = 255 - m_inImageB[i][k];
		}
	}
}


void CColorImageProcessDoc::OnImageGamma()
{
	// TODO: 여기에 구현 코드 추가.

	// 입력값 받기
	CConstantDialog dlg;
	if (dlg.DoModal() != IDOK)	return;
	double val = dlg.m_constant;


	// 기존 메모리 해제
	OnFreeOutImage();

	// 메모리 할당
	m_outH = m_inH;
	m_outW = m_inW;
	OnMallocOutImage();

	// 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageR[i][k] = (unsigned const)(255 * pow(m_inImageR[i][k] / 255.0f, val));
			m_outImageG[i][k] = (unsigned const)(255 * pow(m_inImageG[i][k] / 255.0f, val));
			m_outImageB[i][k] = (unsigned const)(255 * pow(m_inImageB[i][k] / 255.0f, val));
		}
	}
}


void CColorImageProcessDoc::OnRotateByDegree()
{
	// TODO: 여기에 구현 코드 추가.
	CConstantDialog dlg;
	if (dlg.DoModal() != IDOK)	return;
	int val = (int)dlg.m_constant;

	OnFreeOutImage();

	double tmp_radian = val % 90 * 3.141592 / 180.0;
	double tmp_radian90 = (90 - val % 90) * 3.141592 / 180.0;

	m_outH = (int)(m_inH * cos(tmp_radian90) + m_inW * cos(tmp_radian));
	m_outW = (int)(m_inW * cos(tmp_radian) + m_inW * cos(tmp_radian90));

	OnMallocOutImage();
	double radian = -val * 3.141592 / 180.0;

	m_tempH = m_outH;
	m_tempW = m_outW;
	OnMallocTempImage();

	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++)
			m_outImageR[y][x] = m_outImageG[y][x] = m_outImageB[y][x] = 255;

	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++)
			m_tempImageR[y][x] = m_tempImageG[y][x] = m_tempImageB[y][x] = 255;

	int dx = (m_outH - m_inH) / 2;
	int dy = (m_outW - m_inW) / 2;
	for (int i = 0; i < m_inH; i++)
		for (int k = 0; k < m_inW; k++) {
			m_tempImageR[i + dx][k + dy] = m_inImageR[i][k];
			m_tempImageG[i + dx][k + dy] = m_inImageG[i][k];
			m_tempImageB[i + dx][k + dy] = m_inImageB[i][k];
		}

	int cx = m_outH / 2;
	int cy = m_outW / 2;

	for (int i = 0; i < m_outH; i++) {
		for (int k = 0; k < m_outW; k++) {
			int oldI = (cos(radian) * (i - cx) + sin(radian) * (k - cy)) + cx;
			int oldK = (-sin(radian) * (i - cx) + cos(radian) * (k - cy)) + cy;
			if (((0 <= oldI) && (oldI < m_outH)) && ((0 <= oldK) && (oldK < m_outW))) {
				m_outImageR[i][k] = m_tempImageR[oldI][oldK];
				m_outImageG[i][k] = m_tempImageG[oldI][oldK];
				m_outImageB[i][k] = m_tempImageB[oldI][oldK];
			}
		}
	}
	OnFreeTempImage();
}


void CColorImageProcessDoc::OnZoomIn()
{
	// TODO: 여기에 구현 코드 추가.
	OnFreeOutImage();

	m_outH = m_inH * 2;
	m_outW = m_inW * 2;
	OnMallocOutImage();

	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = m_inImageR[i/2][k/2];
			m_outImageG[i][k] = m_inImageG[i/2][k/2];
			m_outImageB[i][k] = m_inImageB[i/2][k/2];
		}
}


void CColorImageProcessDoc::OnZoomOut()
{
	// TODO: 여기에 구현 코드 추가.
	OnFreeOutImage();

	m_outH = m_inH / 2;
	m_outW = m_inW / 2;
	OnMallocOutImage();

	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = m_inImageR[i * 2][k * 2];
			m_outImageG[i][k] = m_inImageG[i * 2][k * 2];
			m_outImageB[i][k] = m_inImageB[i * 2][k * 2];

		}
}


void CColorImageProcessDoc::OnCyberpunk()
{
	// TODO: 여기에 구현 코드 추가.
	OnFreeOutImage();

	m_outH = m_inH;
	m_outW = m_inW;
	OnMallocOutImage();

	const int cp_wid = 3;
	const int cp_color = 0;


	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW; k++) {
			m_outImageR[i][k] = m_inImageR[i][k];
			m_outImageG[i][k] = m_inImageG[i][k];
			m_outImageB[i][k] = m_inImageB[i][k];
		}

	for (int i = 0; i < m_outH; i++)
		for (int k = 0; k < m_outW - cp_wid; k++) {
			m_outImageR[i][k + cp_wid] = m_inImageR[i][k];
			m_outImageG[i][k + cp_wid] = m_inImageG[i][k];
		}

		for (int i = 0; i < m_outH; i++)
			for (int k = cp_wid; k < m_outW; k++) {
				m_outImageG[i][k-cp_wid] = m_inImageG[i][k];
				m_outImageB[i][k-cp_wid] = m_inImageB[i][k];
			}

}

void CColorImageProcessDoc::OnAvgBlur()
{
	// TODO: 여기에 구현 코드 추가.
	CConstantDialog dlg;
	if (dlg.DoModal() != IDOK)	return;
	int val = (int)dlg.m_constant;

	// 마스크 생성
	if (val < 3) val = 3;
	if (val > 9) val = 9;
	if(val % 2 == 0) val++;

	m_ksize = val;
	m_mask = OnMalloc2DMask(m_ksize);
	OnSetAvgMask(m_ksize);


	// 출력 생성
	OnFreeOutImage();

	m_outH = m_inH;
	m_outW = m_inW;
	OnMallocOutImage();

	// 임시 이미지 생성
	m_dimH = m_inH + m_ksize - 1;
	m_dimW = m_inW + m_ksize - 1;
	OnMallocDoubleImage();

	for (int y = 0; y < m_dimH; y++)
		for (int x = 0; x < m_dimW; x++) {
			m_doubleImageR[y][x] = 127;
			m_doubleImageG[y][x] = 127;
			m_doubleImageB[y][x] = 127;
		}

	for (int y = int(m_ksize / 2); y < m_inH + (int)(m_ksize / 2); y++)
		for (int x = int(m_ksize / 2); x < m_inW + (int)(m_ksize / 2); x++) {
			m_doubleImageR[y][x] = m_inImageR[y - (int)(m_ksize / 2)][x - (int)(m_ksize / 2)];
			m_doubleImageG[y][x] = m_inImageG[y - (int)(m_ksize / 2)][x - (int)(m_ksize / 2)];
			m_doubleImageB[y][x] = m_inImageB[y - (int)(m_ksize / 2)][x - (int)(m_ksize / 2)];
		}

	double SR, SB, SG;
	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			SR = SB = SG = 0.0;
			for (int i = 0; i < m_ksize; i++) {
				for (int j = 0; j < m_ksize; j++) {
					SR += m_mask[i][j] * m_doubleImageR[y + i][x + j];
					SG += m_mask[i][j] * m_doubleImageG[y + i][x + j];
					SB += m_mask[i][j] * m_doubleImageB[y + i][x + j];
				}
			}
			if (SR < 0.0) m_outImageR[y][x] = 0;
			else if (SR > 255.0) m_outImageR[y][x] = 255;
			else m_outImageR[y][x] = (unsigned char)SR;

			if (SG < 0.0) m_outImageG[y][x] = 0;
			else if (SG > 255.0) m_outImageG[y][x] = 255;
			else m_outImageG[y][x] = (unsigned char)SG;

			if (SB < 0.0) m_outImageB[y][x] = 0;
			else if (SB > 255.0) m_outImageB[y][x] = 255;
			else m_outImageB[y][x] = (unsigned char)SB;
		}

	// 임시 그림 free
	OnFreeMask();
	OnFreeDoubleImage();
}


void CColorImageProcessDoc::OnSetAvgMask(int ksize)
{
	// TODO: 여기에 구현 코드 추가.
	for (int i = 0; i < ksize; i++)
		for (int k = 0; k < ksize; k++)
			m_mask[i][k] = 1.0 / ksize / ksize;
}

void CColorImageProcessDoc::OnGaussianBlur()
{
	// TODO: 여기에 구현 코드 추가.
	CConstantDialog dlg;
	if (dlg.DoModal() != IDOK)	return;
	int val = (int)dlg.m_constant;

	// 마스크 생성
	if (val < 3) val = 3;
	if (val > 9) val = 9;
	if (val % 2 == 0) val++;

	m_ksize = val;
	m_mask = OnMalloc2DMask(m_ksize);
	OnSetGaussianMask(m_ksize);


	// 출력 생성
	OnFreeOutImage();

	m_outH = m_inH;
	m_outW = m_inW;
	OnMallocOutImage();

	// 임시 이미지 생성
	m_dimH = m_inH + m_ksize - 1;
	m_dimW = m_inW + m_ksize - 1;
	OnMallocDoubleImage();

	for (int y = 0; y < m_dimH; y++)
		for (int x = 0; x < m_dimW; x++) {
			m_doubleImageR[y][x] = 127;
			m_doubleImageG[y][x] = 127;
			m_doubleImageB[y][x] = 127;
		}

	for (int y = int(m_ksize / 2); y < m_inH + (int)(m_ksize / 2); y++)
		for (int x = int(m_ksize / 2); x < m_inW + (int)(m_ksize / 2); x++) {
			m_doubleImageR[y][x] = m_inImageR[y - (int)(m_ksize / 2)][x - (int)(m_ksize / 2)];
			m_doubleImageG[y][x] = m_inImageG[y - (int)(m_ksize / 2)][x - (int)(m_ksize / 2)];
			m_doubleImageB[y][x] = m_inImageB[y - (int)(m_ksize / 2)][x - (int)(m_ksize / 2)];
		}

	double SR, SB, SG;
	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			SR = SB = SG = 0.0;
			for (int i = 0; i < m_ksize; i++) {
				for (int j = 0; j < m_ksize; j++) {
					SR += m_mask[i][j] * m_doubleImageR[y + i][x + j];
					SG += m_mask[i][j] * m_doubleImageG[y + i][x + j];
					SB += m_mask[i][j] * m_doubleImageB[y + i][x + j];
				}
			}
			if (SR < 0.0) m_outImageR[y][x] = 0;
			else if (SR > 255.0) m_outImageR[y][x] = 255;
			else m_outImageR[y][x] = (unsigned char)SR;

			if (SG < 0.0) m_outImageG[y][x] = 0;
			else if (SG > 255.0) m_outImageG[y][x] = 255;
			else m_outImageG[y][x] = (unsigned char)SG;

			if (SB < 0.0) m_outImageB[y][x] = 0;
			else if (SB > 255.0) m_outImageB[y][x] = 255;
			else m_outImageB[y][x] = (unsigned char)SB;
		}

	// 임시 그림 free
	OnFreeMask();
	OnFreeDoubleImage();
}


void CColorImageProcessDoc::OnSetGaussianMask(int ksize)
{
	// TODO: 여기에 구현 코드 추가.
	for (int y = 0 - int(m_ksize / 2); y < m_ksize / 2 + 1; y++)
		for (int x = 0 - int(m_ksize / 2); x < m_ksize / 2 + 1; x++) {
			m_mask[y + int(m_ksize / 2)][x + int(m_ksize / 2)] = 
				(1.0 / (2.0 * (M_PI))) * pow(M_E, (0.0 - ((double)x * x / 2 + (double)y * y / 2.0)));
		}
}

void CColorImageProcessDoc::OnEmbossImage()
{
	// TODO: 여기에 구현 코드 추가.
	// 마스크 생성
	double mask[3][3] = { {1, 0, -1}, {1, 0, -1}, {1, 0, -1} };

	// 출력 생성
	OnFreeOutImage();

	m_outH = m_inH;
	m_outW = m_inW;
	OnMallocOutImage();

	// 임시 이미지 생성
	m_ksize = 3;
	m_dimH = m_inH + m_ksize - 1;
	m_dimW = m_inW + m_ksize - 1;
	OnMallocDoubleImage();

	for (int y = 0; y < m_dimH; y++)
		for (int x = 0; x < m_dimW; x++) {
			m_doubleImageR[y][x] = 127;
		}
	
	for (int y = 0; y < m_inH; y++)
		for (int x = 0; x < m_inW; x++) {
			m_outImageB[y][x] = (unsigned char)((m_inImageR[y][x] + m_inImageG[y][x] + m_inImageB[y][x]) / 3);
		}

	for (int y = int(m_ksize / 2); y < m_inH + (int)(m_ksize / 2); y++)
		for (int x = int(m_ksize / 2); x < m_inW + (int)(m_ksize / 2); x++) {
			m_doubleImageR[y][x] = m_outImageB[y - (int)(m_ksize / 2)][x - (int)(m_ksize / 2)];
		}

	double SR;
	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			SR = 0.0;
			for (int i = 0; i < m_ksize; i++) {
				for (int j = 0; j < m_ksize; j++) {
					SR += mask[i][j] * m_doubleImageR[y + i][x + j];
				}
			}
			SR += 127;
			if (SR > 255) SR = 255;
			else if (SR < 0) SR = 0;
			m_outImageR[y][x] = m_outImageG[y][x] = m_outImageB[y][x] = SR;
		}

	// 임시 그림 free
	OnFreeDoubleImage();
}

void CColorImageProcessDoc::OnPrewittXedge()
{
	// TODO: 여기에 구현 코드 추가.
	// 마스크 생성
	double mask[3][3] = { {-1, -1, -1}, {0, 0, 0}, {1, 1, 1} };

	// 출력 생성
	OnFreeOutImage();

	m_outH = m_inH;
	m_outW = m_inW;
	OnMallocOutImage();

	// 임시 이미지 생성
	m_ksize = 3;
	m_dimH = m_inH + m_ksize - 1;
	m_dimW = m_inW + m_ksize - 1;
	OnMallocDoubleImage();

	for (int y = 0; y < m_dimH; y++)
		for (int x = 0; x < m_dimW; x++) {
			m_doubleImageR[y][x] = 127;
		}

	for (int y = 0; y < m_inH; y++)
		for (int x = 0; x < m_inW; x++) {
			m_outImageB[y][x] = (unsigned char)((m_inImageR[y][x] + m_inImageG[y][x] + m_inImageB[y][x]) / 3);
		}

	for (int y = int(m_ksize / 2); y < m_inH + (int)(m_ksize / 2); y++)
		for (int x = int(m_ksize / 2); x < m_inW + (int)(m_ksize / 2); x++) {
			m_doubleImageR[y][x] = m_outImageB[y - (int)(m_ksize / 2)][x - (int)(m_ksize / 2)];
		}

	double SR;
	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			SR = 0.0;
			for (int i = 0; i < m_ksize; i++) {
				for (int j = 0; j < m_ksize; j++) {
					SR += mask[i][j] * m_doubleImageR[y + i][x + j];
				}
			}
			if (SR > 255) SR = 255;
			else if (SR < 0) SR = 0;
			m_outImageR[y][x] = m_outImageG[y][x] = m_outImageB[y][x] = SR;
		}

	// 임시 그림 free
	OnFreeDoubleImage();
}


void CColorImageProcessDoc::OnPrewittYedge()
{
	// TODO: 여기에 구현 코드 추가.
	// 마스크 생성
	double mask[3][3] = { {-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1} };

	// 출력 생성
	OnFreeOutImage();

	m_outH = m_inH;
	m_outW = m_inW;
	OnMallocOutImage();

	// 임시 이미지 생성
	m_ksize = 3;
	m_dimH = m_inH + m_ksize - 1;
	m_dimW = m_inW + m_ksize - 1;
	OnMallocDoubleImage();

	for (int y = 0; y < m_dimH; y++)
		for (int x = 0; x < m_dimW; x++) {
			m_doubleImageR[y][x] = 127;
		}

	for (int y = 0; y < m_inH; y++)
		for (int x = 0; x < m_inW; x++) {
			m_outImageB[y][x] = (unsigned char)((m_inImageR[y][x] + m_inImageG[y][x] + m_inImageB[y][x]) / 3);
		}

	for (int y = int(m_ksize / 2); y < m_inH + (int)(m_ksize / 2); y++)
		for (int x = int(m_ksize / 2); x < m_inW + (int)(m_ksize / 2); x++) {
			m_doubleImageR[y][x] = m_outImageB[y - (int)(m_ksize / 2)][x - (int)(m_ksize / 2)];
		}

	double SR;
	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			SR = 0.0;
			for (int i = 0; i < m_ksize; i++) {
				for (int j = 0; j < m_ksize; j++) {
					SR += mask[i][j] * m_doubleImageR[y + i][x + j];
				}
			}
			if (SR > 255) SR = 255;
			else if (SR < 0) SR = 0;
			m_outImageR[y][x] = m_outImageG[y][x] = m_outImageB[y][x] = SR;
		}

	// 임시 그림 free
	OnFreeDoubleImage();
}


int CColorImageProcessDoc::OnSobleXedge()
{
	// TODO: 여기에 구현 코드 추가.
	// 마스크 생성
	double mask[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };

	// 출력 생성
	OnFreeOutImage();

	m_outH = m_inH;
	m_outW = m_inW;
	OnMallocOutImage();

	// 임시 이미지 생성
	m_ksize = 3;
	m_dimH = m_inH + m_ksize - 1;
	m_dimW = m_inW + m_ksize - 1;
	OnMallocDoubleImage();

	for (int y = 0; y < m_dimH; y++)
		for (int x = 0; x < m_dimW; x++) {
			m_doubleImageR[y][x] = 127;
		}

	for (int y = 0; y < m_inH; y++)
		for (int x = 0; x < m_inW; x++) {
			m_outImageB[y][x] = (unsigned char)((m_inImageR[y][x] + m_inImageG[y][x] + m_inImageB[y][x]) / 3);
		}

	for (int y = int(m_ksize / 2); y < m_inH + (int)(m_ksize / 2); y++)
		for (int x = int(m_ksize / 2); x < m_inW + (int)(m_ksize / 2); x++) {
			m_doubleImageR[y][x] = m_outImageB[y - (int)(m_ksize / 2)][x - (int)(m_ksize / 2)];
		}

	double SR;
	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			SR = 0.0;
			for (int i = 0; i < m_ksize; i++) {
				for (int j = 0; j < m_ksize; j++) {
					SR += mask[i][j] * m_doubleImageR[y + i][x + j];
				}
			}
			if (SR > 255) SR = 255;
			else if (SR < 0) SR = 0;
			m_outImageR[y][x] = m_outImageG[y][x] = m_outImageB[y][x] = SR;
		}

	// 임시 그림 free
	OnFreeDoubleImage();
	return 0;
}


void CColorImageProcessDoc::OnSobleYedge()
{
	// TODO: 여기에 구현 코드 추가.
	// 마스크 생성
	double mask[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };

	// 출력 생성
	OnFreeOutImage();

	m_outH = m_inH;
	m_outW = m_inW;
	OnMallocOutImage();

	// 임시 이미지 생성
	m_ksize = 3;
	m_dimH = m_inH + m_ksize - 1;
	m_dimW = m_inW + m_ksize - 1;
	OnMallocDoubleImage();

	for (int y = 0; y < m_dimH; y++)
		for (int x = 0; x < m_dimW; x++) {
			m_doubleImageR[y][x] = 127;
		}

	for (int y = 0; y < m_inH; y++)
		for (int x = 0; x < m_inW; x++) {
			m_outImageB[y][x] = (unsigned char)((m_inImageR[y][x] + m_inImageG[y][x] + m_inImageB[y][x]) / 3);
		}

	for (int y = int(m_ksize / 2); y < m_inH + (int)(m_ksize / 2); y++)
		for (int x = int(m_ksize / 2); x < m_inW + (int)(m_ksize / 2); x++) {
			m_doubleImageR[y][x] = m_outImageB[y - (int)(m_ksize / 2)][x - (int)(m_ksize / 2)];
		}

	double SR;
	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			SR = 0.0;
			for (int i = 0; i < m_ksize; i++) {
				for (int j = 0; j < m_ksize; j++) {
					SR += mask[i][j] * m_doubleImageR[y + i][x + j];
				}
			}
			if (SR > 255) SR = 255;
			else if (SR < 0) SR = 0;
			m_outImageR[y][x] = m_outImageG[y][x] = m_outImageB[y][x] = SR;
		}

	// 임시 그림 free
	OnFreeDoubleImage();
}


void CColorImageProcessDoc::OnLaplacian1dEdge()
{
	// TODO: 여기에 구현 코드 추가.
	// 마스크 생성
	double mask[3][3] = { {0, 1, 0}, {1, -4, 1}, {0, 1, 0} };

	// 출력 생성
	OnFreeOutImage();

	m_outH = m_inH;
	m_outW = m_inW;
	OnMallocOutImage();

	// 임시 이미지 생성
	m_ksize = 3;
	m_dimH = m_inH + m_ksize - 1;
	m_dimW = m_inW + m_ksize - 1;
	OnMallocDoubleImage();

	for (int y = 0; y < m_dimH; y++)
		for (int x = 0; x < m_dimW; x++) {
			m_doubleImageR[y][x] = 127;
		}

	for (int y = 0; y < m_inH; y++)
		for (int x = 0; x < m_inW; x++) {
			m_outImageB[y][x] = (unsigned char)((m_inImageR[y][x] + m_inImageG[y][x] + m_inImageB[y][x]) / 3);
		}

	for (int y = int(m_ksize / 2); y < m_inH + (int)(m_ksize / 2); y++)
		for (int x = int(m_ksize / 2); x < m_inW + (int)(m_ksize / 2); x++) {
			m_doubleImageR[y][x] = m_outImageB[y - (int)(m_ksize / 2)][x - (int)(m_ksize / 2)];
		}

	double SR;
	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			SR = 0.0;
			for (int i = 0; i < m_ksize; i++) {
				for (int j = 0; j < m_ksize; j++) {
					SR += mask[i][j] * m_doubleImageR[y + i][x + j];
				}
			}
			if (SR > 255) SR = 255;
			else if (SR < 0) SR = 0;
			m_outImageR[y][x] = m_outImageG[y][x] = m_outImageB[y][x] = SR;
		}

	// 임시 그림 free
	OnFreeDoubleImage();
}


void CColorImageProcessDoc::OnLaplacian2dEdge()
{
	// TODO: 여기에 구현 코드 추가.
	// 마스크 생성
	double mask[3][3] = { {1, 1, 1}, {1, -8, 1}, {1, 1, 1} };

	// 출력 생성
	OnFreeOutImage();

	m_outH = m_inH;
	m_outW = m_inW;
	OnMallocOutImage();

	// 임시 이미지 생성
	m_ksize = 3;
	m_dimH = m_inH + m_ksize - 1;
	m_dimW = m_inW + m_ksize - 1;
	OnMallocDoubleImage();

	for (int y = 0; y < m_dimH; y++)
		for (int x = 0; x < m_dimW; x++) {
			m_doubleImageR[y][x] = 127;
		}

	for (int y = 0; y < m_inH; y++)
		for (int x = 0; x < m_inW; x++) {
			m_outImageB[y][x] = (unsigned char)((m_inImageR[y][x] + m_inImageG[y][x] + m_inImageB[y][x]) / 3);
		}

	for (int y = int(m_ksize / 2); y < m_inH + (int)(m_ksize / 2); y++)
		for (int x = int(m_ksize / 2); x < m_inW + (int)(m_ksize / 2); x++) {
			m_doubleImageR[y][x] = m_outImageB[y - (int)(m_ksize / 2)][x - (int)(m_ksize / 2)];
		}

	double SR;
	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			SR = 0.0;
			for (int i = 0; i < m_ksize; i++) {
				for (int j = 0; j < m_ksize; j++) {
					SR += mask[i][j] * m_doubleImageR[y + i][x + j];
				}
			}
			if (SR > 255) SR = 255;
			else if (SR < 0) SR = 0;
			m_outImageR[y][x] = m_outImageG[y][x] = m_outImageB[y][x] = SR;
		}

	// 임시 그림 free
	OnFreeDoubleImage();
}


void CColorImageProcessDoc::OnLaplacianOfGaussian()
{
	// TODO: 여기에 구현 코드 추가.
	CConstantDialog dlg;
	if (dlg.DoModal() != IDOK)	return;
	int val = (int)dlg.m_constant;

	// 마스크 생성
	if (val < 3) val = 3;
	if (val > 9) val = 9;
	if (val % 2 == 0) val++;

	m_ksize = val;
	m_mask = OnMalloc2DMask(m_ksize);
	OnSetGaussianMask(m_ksize);


	// 출력 생성
	OnFreeOutImage();

	m_outH = m_inH;
	m_outW = m_inW;
	OnMallocOutImage();

	// 임시 이미지 생성
	m_dimH = m_inH + m_ksize - 1;
	m_dimW = m_inW + m_ksize - 1;
	OnMallocDoubleImage();

	for (int y = 0; y < m_dimH; y++)
		for (int x = 0; x < m_dimW; x++) {
			m_doubleImageR[y][x] = 127;
			m_doubleImageG[y][x] = 127;
			m_doubleImageB[y][x] = 127;
		}

	for (int y = int(m_ksize / 2); y < m_inH + (int)(m_ksize / 2); y++)
		for (int x = int(m_ksize / 2); x < m_inW + (int)(m_ksize / 2); x++) {
			m_doubleImageR[y][x] = m_inImageR[y - (int)(m_ksize / 2)][x - (int)(m_ksize / 2)];
			m_doubleImageG[y][x] = m_inImageG[y - (int)(m_ksize / 2)][x - (int)(m_ksize / 2)];
			m_doubleImageB[y][x] = m_inImageB[y - (int)(m_ksize / 2)][x - (int)(m_ksize / 2)];
		}

	double SR, SB, SG;
	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			SR = SB = SG = 0.0;
			for (int i = 0; i < m_ksize; i++) {
				for (int j = 0; j < m_ksize; j++) {
					SR += m_mask[i][j] * m_doubleImageR[y + i][x + j];
					SG += m_mask[i][j] * m_doubleImageG[y + i][x + j];
					SB += m_mask[i][j] * m_doubleImageB[y + i][x + j];
				}
			}
			if (SR < 0.0) m_outImageR[y][x] = 0;
			else if (SR > 255.0) m_outImageR[y][x] = 255;
			else m_outImageR[y][x] = (unsigned char)SR;

			if (SG < 0.0) m_outImageG[y][x] = 0;
			else if (SG > 255.0) m_outImageG[y][x] = 255;
			else m_outImageG[y][x] = (unsigned char)SG;

			if (SB < 0.0) m_outImageB[y][x] = 0;
			else if (SB > 255.0) m_outImageB[y][x] = 255;
			else m_outImageB[y][x] = (unsigned char)SB;
		}

	// 임시 그림 free
	OnFreeMask();
	OnFreeDoubleImage();

	// 이미지 복사
	OnFreeTempImage();

	m_tempH = m_outH;
	m_tempW = m_outW;
	OnMallocTempImage();

	for(int y = 0; y < m_tempH; y++)
		for (int x = 0; x < m_tempW; x++) {
			m_tempImageR[y][x] = m_outImageR[y][x];
			m_tempImageG[y][x] = m_outImageG[y][x];
			m_tempImageB[y][x] = m_outImageB[y][x];
		}

	// 마스크 생성
	double mask[3][3] = { {1, 1, 1}, {1, -8, 1}, {1, 1, 1} };

	// 출력 생성
	OnFreeOutImage();

	m_outH = m_inH;
	m_outW = m_inW;
	OnMallocOutImage();

	// 임시 이미지 생성
	m_ksize = 3;
	m_dimH = m_inH + m_ksize - 1;
	m_dimW = m_inW + m_ksize - 1;
	OnMallocDoubleImage();

	for (int y = 0; y < m_dimH; y++)
		for (int x = 0; x < m_dimW; x++) {
			m_doubleImageR[y][x] = 127;
		}

	for (int y = 0; y < m_tempH; y++)
		for (int x = 0; x < m_tempW; x++) {
			m_outImageB[y][x] = (unsigned char)((m_tempImageR[y][x] + m_tempImageG[y][x] + m_tempImageB[y][x]) / 3);
		}

	for (int y = int(m_ksize / 2); y < m_tempH + (int)(m_ksize / 2); y++)
		for (int x = int(m_ksize / 2); x < m_tempW + (int)(m_ksize / 2); x++) {
			m_doubleImageR[y][x] = m_outImageB[y - (int)(m_ksize / 2)][x - (int)(m_ksize / 2)];
		}

	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			SR = 0.0;
			for (int i = 0; i < m_ksize; i++) {
				for (int j = 0; j < m_ksize; j++) {
					SR += mask[i][j] * m_doubleImageR[y + i][x + j];
				}
			}
			if (SR > 255) SR = 255;
			else if (SR < 0) SR = 0;
			m_outImageR[y][x] = m_outImageG[y][x] = m_outImageB[y][x] = SR;
		}

	// 임시 그림 free
	OnFreeDoubleImage();
}



void CColorImageProcessDoc::OnHistStrech()
{
	// TODO: 여기에 구현 코드 추가.
	OnFreeOutImage();

	m_outH = m_inH;
	m_outW = m_inW;
	OnMallocOutImage();

	unsigned char rHIGH = 0;
	unsigned char rLOW = 255;
	unsigned char gHIGH = 0;
	unsigned char gLOW = 255;
	unsigned char bHIGH = 0;
	unsigned char bLOW = 255;

	for (int y = 0; y < m_inH; y++)
		for (int x = 0; x < m_inW; x++) {
			if (m_inImageR[y][x] > rHIGH) rHIGH = m_inImageR[y][x];
			if (m_inImageR[y][x] < rLOW) rLOW = m_inImageR[y][x];

			if (m_inImageG[y][x] > gHIGH) gHIGH = m_inImageG[y][x];
			if (m_inImageG[y][x] < gLOW) gLOW = m_inImageG[y][x];

			if (m_inImageB[y][x] > bHIGH) bHIGH = m_inImageB[y][x];
			if (m_inImageB[y][x] < bLOW) bLOW = m_inImageB[y][x];
		}



	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			m_outImageR[y][x] = (unsigned const)((m_inImageR[y][x] - rLOW) / (float)(rHIGH - rLOW) * 255);
			m_outImageG[y][x] = (unsigned const)((m_inImageG[y][x] - gLOW) / (float)(rHIGH - gLOW) * 255);
			m_outImageB[y][x] = (unsigned const)((m_inImageB[y][x] - bLOW) / (float)(rHIGH - bLOW) * 255);
		}
}


void CColorImageProcessDoc::OnHistEqual()
{
	// TODO: 여기에 구현 코드 추가.
	OnFreeOutImage();

	m_outH = m_inH;
	m_outW = m_inW;
	OnMallocOutImage();

	int HistR[256] = { 0, };
	int HistG[256] = { 0, };
	int HistB[256] = { 0, };

	int HistSumR[256] = { 0, };
	int HistSumG[256] = { 0, };
	int HistSumB[256] = { 0, };

	for (int y = 0; y < m_inH; y++)
		for (int x = 0; x < m_inW; x++) {
			HistR[m_inImageR[y][x]]++;
			HistG[m_inImageG[y][x]]++;
			HistB[m_inImageB[y][x]]++;
		}

	for (int i = 1; i < 256; i++) {
		HistSumR[i] = HistSumR[i - 1] + HistR[i];
		HistSumG[i] = HistSumG[i - 1] + HistG[i];
		HistSumB[i] = HistSumB[i - 1] + HistB[i];
	}

	double HistNormR[256] = { 1.0, };
	double HistNormG[256] = { 1.0, };
	double HistNormB[256] = { 1.0, };

	for (int i = 0; i < 256; i++)
		HistNormR[i] = HistNormB[i] = HistNormG[i] = 1.0;

	for (int i = 0; i < 256; i++) {
		HistNormR[i] = HistSumR[i] * (1.0 / (m_inH * m_inW)) * 255.0;
		HistNormG[i] = HistSumG[i] * (1.0 / (m_inH * m_inW)) * 255.0;
		HistNormB[i] = HistSumB[i] * (1.0 / (m_inH * m_inW)) * 255.0;
	}

	for (int y = 0; y < m_outH; y++) {
		for (int x = 0; x < m_outW; x++) {
			m_outImageR[y][x] = (unsigned char)HistNormR[m_inImageR[y][x]];
			m_outImageG[y][x] = (unsigned char)HistNormG[m_inImageG[y][x]];
			m_outImageB[y][x] = (unsigned char)HistNormB[m_inImageB[y][x]];
		}
	}
}


void CColorImageProcessDoc::OnDeResolution()
{
	// TODO: 여기에 구현 코드 추가.
	// 기존 메모리 해제
	OnFreeOutImage();

	// 메모리 할당
	m_outH = m_inH;
	m_outW = m_inW;
	OnMallocOutImage();
	volatile double temp = 86;
	int val = 83;

	// 영상처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_outImageR[i][k] = (unsigned char)(m_inImageR[i][k] / temp) * temp;
			m_outImageG[i][k] = (unsigned char)(m_inImageG[i][k] / temp) * temp;
			m_outImageB[i][k] = (unsigned char)(m_inImageB[i][k] / temp) * temp;
		}
	}
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			if (m_outImageR[i][k] != 0)
				if (m_outImageR[i][k] <= 255 - val) m_outImageR[i][k] += val;
			if (m_outImageG[i][k] != 0)
				if (m_outImageG[i][k] <= 255 - val) m_outImageG[i][k] += val;
			if (m_outImageB[i][k] != 0)
				if (m_outImageB[i][k] <= 255 - val) m_outImageB[i][k] += val;
		}
	}

}