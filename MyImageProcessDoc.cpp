
// MyImageProcessDoc.cpp: CMyImageProcessDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MyImageProcess.h"
#endif

#include "MyImageProcessDoc.h"

#include <propkey.h>
#include "ConstantDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMyImageProcessDoc

IMPLEMENT_DYNCREATE(CMyImageProcessDoc, CDocument)

BEGIN_MESSAGE_MAP(CMyImageProcessDoc, CDocument)
	ON_COMMAND(ID_GammaImage, &CMyImageProcessDoc::OnGammaImage)
	ON_COMMAND(ID_PrewittXEdge, &CMyImageProcessDoc::OnPrewittXEdge)
END_MESSAGE_MAP()


// CMyImageProcessDoc 생성/소멸

CMyImageProcessDoc::CMyImageProcessDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CMyImageProcessDoc::~CMyImageProcessDoc()
{
}

BOOL CMyImageProcessDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CMyImageProcessDoc serialization

void CMyImageProcessDoc::Serialize(CArchive& ar)
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
void CMyImageProcessDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CMyImageProcessDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMyImageProcessDoc::SetSearchContent(const CString& value)
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

// CMyImageProcessDoc 진단

#ifdef _DEBUG
void CMyImageProcessDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMyImageProcessDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMyImageProcessDoc 명령


unsigned char** CMyImageProcessDoc::OnMalloc2D(int H, int W, int initVal)
{
	// TODO: 여기에 구현 코드 추가.
	unsigned char** retMem;

	retMem = new unsigned char* [H];
	for (int i = 0; i < H; i++)
		retMem[i] = new unsigned char[W];
	return retMem;
}


void CMyImageProcessDoc::OnFree2D(unsigned char** memory, int H)
{
	// TODO: 여기에 구현 코드 추가.
	if (memory == nullptr) return;

	for (int i = 0; i < H; i++)
		delete memory[i];

	delete[] memory;
}

double** CMyImageProcessDoc::OnMallocMask(int K)
{
	double** retMem;

	retMem = new double* [K];
	for (int i = 0; i < K; i++)
		retMem[i] = new double[K];
	return retMem;
}


void CMyImageProcessDoc::OnFreeMask(double** mask, int K)
{
	// TODO: 여기에 구현 코드 추가.
	if (mask == nullptr) return;

	for (int i = 0; i < K; i++)
		delete mask[i];

	delete[] mask;
}

BOOL CMyImageProcessDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CFile File;
	File.Open(lpszPathName, CFile::modeRead | CFile::typeBinary);
	// 메모리 헤제 관련값 초기화

	ULONGLONG fLength = File.GetLength();
	// 입력 이미지 높이 폭 + 메모리
	OnFree2D(m_inImage, m_inH);
	m_inImage = NULL;
	m_inH = m_inW = 0;
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;
	
	m_inH = m_inW = (int)sqrt((double)fLength);
	m_inImage = OnMalloc2D(m_inH, m_inW, 0);
		
	// 파일 --> 메모리
	for (int i = 0; i < m_inH; i++)
		File.Read(m_inImage[i], m_inW);



	return TRUE;
}

BOOL CMyImageProcessDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CFile File;
	CFileDialog SaveDlg(FALSE, L"raw", NULL, OFN_HIDEREADONLY);

	if (SaveDlg.DoModal() == IDOK) {
		File.Open(SaveDlg.GetPathName(), CFile::modeCreate | CFile::modeWrite);
		for (int y = 0; y < m_outH; y++)
			File.Write(m_outImage[y], m_outW);
		File.Close();
	}

	return CDocument::OnSaveDocument(lpszPathName);
}


void CMyImageProcessDoc::OnCloseDocument()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	OnFree2D(m_inImage, m_inH);
	m_inImage = NULL;
	m_inH = m_inW = 0;
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	CDocument::OnCloseDocument();
}


void CMyImageProcessDoc::OnEqualImage()
{
	// TODO: 여기에 구현 코드 추가.
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);



	for (int y = 0; y < m_inH; y++)
		for (int x = 0; x < m_inW; x++) {
			m_outImage[y][x] = m_inImage[y][x];
		}

}


void CMyImageProcessDoc::OnAddimage()
{
	CConstantDlg dlg;
	if (dlg.DoModal() != IDOK)	return;

	// TODO: 여기에 구현 코드 추가.
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);

	int val = (int)dlg.m_constant;


	for (int y = 0; y < m_inH; y++)
		for (int x = 0; x < m_inW; x++) {
			if (m_inImage[y][x] + val > 255) m_outImage[y][x] = 255;
			else if (m_inImage[y][x] + val < 0) m_outImage = 0;
			else m_outImage[y][x] = m_inImage[y][x] + val;
		}

}


void CMyImageProcessDoc::OnInverseImage()
{
	// TODO: 여기에 구현 코드 추가.
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);


	for (int y = 0; y < m_inH; y++)
		for (int x = 0; x < m_inW; x++) {
			m_outImage[y][x] = 255 - m_inImage[y][x];
		}

}


void CMyImageProcessDoc::OnGammaImage()
{
	CConstantDlg dlg;
	if (dlg.DoModal() != IDOK)	return;

	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);
	double val = dlg.m_constant;


	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			m_outImage[y][x] = (unsigned const)(255 * pow(m_inImage[y][x] / 255.0f, val));
		}
}


void CMyImageProcessDoc::HistStretch()
{
	// TODO: 여기에 구현 코드 추가.

	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);

	unsigned char HIGH = 0;
	unsigned char LOW = 255;

	for (int y = 0; y < m_inH; y++)
		for (int x = 0; x < m_inW; x++) {
			if (m_inImage[y][x] > HIGH) HIGH = m_inImage[y][x];
			if (m_inImage[y][x] < LOW) LOW = m_inImage[y][x];
		}



	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			m_outImage[y][x] = (unsigned const)((m_inImage[y][x] - LOW) / (float)(HIGH - LOW) * 255);
		}

}


void CMyImageProcessDoc::OnHistEqual()
{
	// TODO: 여기에 구현 코드 추가.
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);

	int Hist[256] = { 0, };
	int HistSum[256] = { 0, };

	for (int y = 0; y < m_inH; y++)
		for (int x = 0; x < m_inW; x++) 
			Hist[m_inImage[y][x]]++;

	for (int i = 1; i < 256; i++)
		HistSum[i] = HistSum[i - 1] + Hist[i];

	double HistNorm[256] = { 1.0, };
	for (int i = 0; i < 256; i++) {
		HistNorm[i] = HistSum[i] * (1.0 / (m_inH * m_inW)) * 255.0;
	}

	for (int y = 0; y < m_outH; y++) {
		for (int x = 0; x < m_outW; x++) {
			m_outImage[y][x] = (unsigned char)HistNorm[m_inImage[y][x]];
		}
	}
}


void CMyImageProcessDoc::OnRotateImage()
{
	// TODO: 여기에 구현 코드 추가.
	CConstantDlg dlg;
	if (dlg.DoModal() != IDOK)	return;

	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);
	double val = dlg.m_constant;


	double radian = -val * 3.141592 / 180.0;

	int cx = m_outH / 2;
	int cy = m_outW / 2;

	for (int y = 0; y < m_outH; y++) {
		for (int x = 0; x < m_outW; x++) {
			int xd = y;
			int yd = x;

			int xs = (int)(cos(radian) * (xd - cx) + sin(radian) * (yd - cy));
			int ys = (int)(-sin(radian) * (xd - cx) + cos(radian) * (yd - cy));
			xs += cx;
			ys += cy;

			if ((0 <= xs && xs < m_outH) && (0 <= ys && ys < m_outW))
				m_outImage[xd][yd] = m_inImage[xs][ys];
		}
	}
}


void CMyImageProcessDoc::OnEmbossImage()
{
	CConstantDlg dlg;
	if (dlg.DoModal() != IDOK)	return;

	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);

	//마스크 초기화 및 생성
	int val = (int)dlg.m_constant;

	if (val < 1) val = 3;
	if (val % 2 == 0) val += 1;

	OnFreeMask(m_mask, m_ksize);
	m_mask = NULL;
	m_ksize = val;
	m_mask = OnMallocMask(m_ksize);

	for (int y = 0; y < m_ksize; y++)
		for (int x = 0; x < m_ksize; x++) {
			if (y < x) m_mask[y][x] = -1;
			else if (y > x) m_mask[y][x] = 1;
			else m_mask[y][x] = 0;
		}

	// 임시 이미지 생성
	double** tempImage;
	double** tempOut;
	int tempH = m_inH + m_ksize - 1;
	tempImage = OnMallocMask(tempH);
	tempOut = OnMallocMask(m_outH);

	for (int y = 0; y < tempH; y++)
		for (int x = 0; x < tempH; x++)
			tempImage[y][x] = 127;

	for (int y = int(m_ksize / 2); y < m_inH + (int)(m_ksize / 2); y++)
		for (int x = int(m_ksize / 2); x < m_inW + (int)(m_ksize / 2); x++)
			tempImage[y][x] = m_inImage[y - (int)(m_ksize / 2)][x - (int)(m_ksize / 2)];


	// 회선 연산
	double S;
	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			S = 0.0;
			for (int i = 0; i < m_ksize; i++) {
				for (int j = 0; j < m_ksize; j++)
					S += m_mask[i][j] * tempImage[y + i][x + j];
			}
			tempOut[y][x] = S;
		}

	// 값 보정
	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++)
			tempOut[y][x] += 127;

	// 출력
	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			if (tempOut[y][x] > 255) m_outImage[y][x] = 255;
			else if (tempOut[y][x] < 0) m_outImage[y][x] = 0;
			else m_outImage[y][x] = (unsigned char)tempOut[y][x];
		}
	
	// 임시 그림 free
	OnFreeMask(tempImage, tempH);
	OnFreeMask(tempOut, m_outH);
}


void CMyImageProcessDoc::OnAverageBlur()
{
	// TODO: 여기에 구현 코드 추가.
	CConstantDlg dlg;
	if (dlg.DoModal() != IDOK)	return;

	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);

	//마스크 초기화 및 생성
	int val = (int)dlg.m_constant;

	if (val < 1) val = 3;
	if (val % 2 == 0) val += 1;

	OnFreeMask(m_mask, m_ksize);
	m_mask = NULL;
	m_ksize = val;
	m_mask = OnMallocMask(m_ksize);

	for (int y = 0; y < m_ksize; y++)
		for (int x = 0; x < m_ksize; x++) {
			m_mask[y][x] = 1.0 / m_ksize / m_ksize;
		}

	// 임시 이미지 생성
	double** tempImage;

	int tempH = m_inH + m_ksize - 1;
	tempImage = OnMallocMask(tempH);

	for (int y = 0; y < tempH; y++)
		for (int x = 0; x < tempH; x++)
			tempImage[y][x] = 127;

	for (int y = int(m_ksize / 2); y < m_inH + (int)(m_ksize / 2); y++)
		for (int x = int(m_ksize / 2); x < m_inW + (int)(m_ksize / 2); x++)
			tempImage[y][x] = m_inImage[y - (int)(m_ksize / 2)][x - (int)(m_ksize / 2)];


	// 회선 연산
	double S;
	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			S = 0.0;
			for (int i = 0; i < m_ksize; i++) {
				for (int j = 0; j < m_ksize; j++)
					S += m_mask[i][j] * tempImage[y + i][x + j];
			}
			if (S < 0.0) m_outImage[y][x] = 0;
			else if (S > 255.0) m_outImage[y][x] = 255;
			else m_outImage[y][x] = (unsigned char)S;
		}

	// 임시 그림 free
	OnFreeMask(tempImage, tempH);
}


void CMyImageProcessDoc::OnPrewittXEdge()
{
	// TODO: 여기에 구현 코드 추가.
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);

	//마스크 초기화 및 생성
	double mask[3][3] = { {-1, -1, -1}, {0, 0, 0}, {1, 1, 1} };
	int ksize = 3;

	// 임시 이미지 생성
	double** tempImage;

	int tempH = m_inH + ksize - 1;
	tempImage = OnMallocMask(tempH);

	for (int y = 0; y < tempH; y++)
		for (int x = 0; x < tempH; x++)
			tempImage[y][x] = 127;

	for (int y = int(ksize / 2); y < m_inH + (int)(ksize / 2); y++)
		for (int x = int(ksize / 2); x < m_inW + (int)(ksize / 2); x++)
			tempImage[y][x] = m_inImage[y - (int)(ksize / 2)][x - (int)(ksize / 2)];


	// 회선 연산
	double S;
	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			S = 0.0;
			for (int i = 0; i < ksize; i++) {
				for (int j = 0; j < ksize; j++)
					S += mask[i][j] * tempImage[y + i][x + j];
			}
			if (S < 0.0) m_outImage[y][x] = 0;
			else if (S > 255.0) m_outImage[y][x] = 255;
			else m_outImage[y][x] = (unsigned char)S;
		}

	// 임시 그림 free
	OnFreeMask(tempImage, tempH);
}


void CMyImageProcessDoc::OnPrewittYEdge()
{
	// TODO: 여기에 구현 코드 추가.
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);

	//마스크 초기화 및 생성
	double mask[3][3] = { {-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1} };
	int ksize = 3;

	// 임시 이미지 생성
	double** tempImage;

	int tempH = m_inH + ksize - 1;
	tempImage = OnMallocMask(tempH);

	for (int y = 0; y < tempH; y++)
		for (int x = 0; x < tempH; x++)
			tempImage[y][x] = 127;

	for (int y = int(ksize / 2); y < m_inH + (int)(ksize / 2); y++)
		for (int x = int(ksize / 2); x < m_inW + (int)(ksize / 2); x++)
			tempImage[y][x] = m_inImage[y - (int)(ksize / 2)][x - (int)(ksize / 2)];


	// 회선 연산
	double S;
	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			S = 0.0;
			for (int i = 0; i < ksize; i++) {
				for (int j = 0; j < ksize; j++)
					S += mask[i][j] * tempImage[y + i][x + j];
			}
			if (S < 0.0) m_outImage[y][x] = 0;
			else if (S > 255.0) m_outImage[y][x] = 255;
			else m_outImage[y][x] = (unsigned char)S;
		}

	// 임시 그림 free
	OnFreeMask(tempImage, tempH);
}


void CMyImageProcessDoc::SobelXEdge()
{
	// TODO: 여기에 구현 코드 추가.
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);

	//마스크 초기화 및 생성
	double mask[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };
	int ksize = 3;

	// 임시 이미지 생성
	double** tempImage;

	int tempH = m_inH + ksize - 1;
	tempImage = OnMallocMask(tempH);

	for (int y = 0; y < tempH; y++)
		for (int x = 0; x < tempH; x++)
			tempImage[y][x] = 127;

	for (int y = int(ksize / 2); y < m_inH + (int)(ksize / 2); y++)
		for (int x = int(ksize / 2); x < m_inW + (int)(ksize / 2); x++)
			tempImage[y][x] = m_inImage[y - (int)(ksize / 2)][x - (int)(ksize / 2)];


	// 회선 연산
	double S;
	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			S = 0.0;
			for (int i = 0; i < ksize; i++) {
				for (int j = 0; j < ksize; j++)
					S += mask[i][j] * tempImage[y + i][x + j];
			}
			if (S < 0.0) m_outImage[y][x] = 0;
			else if (S > 255.0) m_outImage[y][x] = 255;
			else m_outImage[y][x] = (unsigned char)S;
		}

	// 임시 그림 free
	OnFreeMask(tempImage, tempH);
}


void CMyImageProcessDoc::OnSobelYEdge()
{
	// TODO: 여기에 구현 코드 추가.
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);

	//마스크 초기화 및 생성
	double mask[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
	int ksize = 3;

	// 임시 이미지 생성
	double** tempImage;

	int tempH = m_inH + ksize - 1;
	tempImage = OnMallocMask(tempH);

	for (int y = 0; y < tempH; y++)
		for (int x = 0; x < tempH; x++)
			tempImage[y][x] = 127;

	for (int y = int(ksize / 2); y < m_inH + (int)(ksize / 2); y++)
		for (int x = int(ksize / 2); x < m_inW + (int)(ksize / 2); x++)
			tempImage[y][x] = m_inImage[y - (int)(ksize / 2)][x - (int)(ksize / 2)];


	// 회선 연산
	double S;
	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			S = 0.0;
			for (int i = 0; i < ksize; i++) {
				for (int j = 0; j < ksize; j++)
					S += mask[i][j] * tempImage[y + i][x + j];
			}
			if (S < 0.0) m_outImage[y][x] = 0;
			else if (S > 255.0) m_outImage[y][x] = 255;
			else m_outImage[y][x] = (unsigned char)S;
		}

	// 임시 그림 free
	OnFreeMask(tempImage, tempH);
}


int CMyImageProcessDoc::OnZoomIn()
{
	// TODO: 여기에 구현 코드 추가.
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	m_outH = m_inH * 2;
	m_outW = m_inW * 2;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);


	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			m_outImage[y][x] = m_inImage[y / 2][x / 2];
		}
	return 0;
}


int CMyImageProcessDoc::OnZoomOut()
{
	// TODO: 여기에 구현 코드 추가.
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	m_outH = m_inH / 2;
	m_outW = m_inW / 2;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);


	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			m_outImage[y][x] = m_inImage[y * 2][x * 2];
		}
	return 0;
}

void CMyImageProcessDoc::OnLaplacian1DEdge()
{
	// TODO: 여기에 구현 코드 추가.
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);

	//마스크 초기화 및 생성
	double mask[3][3] = { {0, 1, 0}, {1, -4, 1}, {0, 1, 0} };
	int ksize = 3;

	// 임시 이미지 생성
	double** tempImage;

	int tempH = m_inH + ksize - 1;
	tempImage = OnMallocMask(tempH);

	for (int y = 0; y < tempH; y++)
		for (int x = 0; x < tempH; x++)
			tempImage[y][x] = 127;

	for (int y = int(ksize / 2); y < m_inH + (int)(ksize / 2); y++)
		for (int x = int(ksize / 2); x < m_inW + (int)(ksize / 2); x++)
			tempImage[y][x] = m_inImage[y - (int)(ksize / 2)][x - (int)(ksize / 2)];


	// 회선 연산
	double S;
	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			S = 0.0;
			for (int i = 0; i < ksize; i++) {
				for (int j = 0; j < ksize; j++)
					S += mask[i][j] * tempImage[y + i][x + j];
			}
			if (S < 0.0) m_outImage[y][x] = 0;
			else if (S > 255.0) m_outImage[y][x] = 255;
			else m_outImage[y][x] = (unsigned char)S;
		}

	// 임시 그림 free
	OnFreeMask(tempImage, tempH);
}


void CMyImageProcessDoc::OnLaplacian2DEdge()
{
	// TODO: 여기에 구현 코드 추가.
	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);

	//마스크 초기화 및 생성
	double mask[3][3] = { {1, 1, 1}, {1, -8, 1}, {1, 1, 1} };
	int ksize = 3;

	// 임시 이미지 생성
	double** tempImage;

	int tempH = m_inH + ksize - 1;
	tempImage = OnMallocMask(tempH);

	for (int y = 0; y < tempH; y++)
		for (int x = 0; x < tempH; x++)
			tempImage[y][x] = 127;

	for (int y = int(ksize / 2); y < m_inH + (int)(ksize / 2); y++)
		for (int x = int(ksize / 2); x < m_inW + (int)(ksize / 2); x++)
			tempImage[y][x] = m_inImage[y - (int)(ksize / 2)][x - (int)(ksize / 2)];


	// 회선 연산
	double S;
	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			S = 0.0;
			for (int i = 0; i < ksize; i++) {
				for (int j = 0; j < ksize; j++)
					S += mask[i][j] * tempImage[y + i][x + j];
			}
			if (S < 0.0) m_outImage[y][x] = 0;
			else if (S > 255.0) m_outImage[y][x] = 255;
			else m_outImage[y][x] = (unsigned char)S;
		}

	// 임시 그림 free
	OnFreeMask(tempImage, tempH);
}


void CMyImageProcessDoc::OnBinByVal()
{
	// TODO: 여기에 구현 코드 추가.
	CConstantDlg dlg;
	if (dlg.DoModal() != IDOK)	return;

	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);
	int val = (int) dlg.m_constant;


	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			if (m_inImage[y][x] > val) m_outImage[y][x] = 255;
			else m_outImage[y][x] = 0;
		}
}


void CMyImageProcessDoc::OnBinByAvg()
{
	// TODO: 여기에 구현 코드 추가.

	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);
	int val = 0;
	int sum = 0;

	for (int y = 0; y < m_inH; y++)
		for (int x = 0; x < m_inW; x++)
			sum += m_inImage[y][x];
	val = sum / m_inH / m_inW;

	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			if (m_inImage[y][x] > val) m_outImage[y][x] = 255;
			else m_outImage[y][x] = 0;
		}
}


void CMyImageProcessDoc::OnBinByMedian()
{
	// TODO: 여기에 구현 코드 추가.

	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);
	int val = 0;
	UINT64 medianCnt[256] = { 0, };
	UINT64 count = 0;

	for (int y = 0; y < m_inH; y++)
		for (int x = 0; x < m_inW; x++)
			medianCnt[m_inImage[y][x]] += 1;

	for (int i = 0; i < 256; i++) {
		count += medianCnt[i];
		if (count > m_inH * m_inW / 2) {
			val = i;
			break;
		}
	}

	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			if (m_inImage[y][x] > val) m_outImage[y][x] = 255;
			else m_outImage[y][x] = 0;
		}
}


void CMyImageProcessDoc::OnGaussianBlur()
{
	// TODO: 여기에 구현 코드 추가.
	CConstantDlg dlg;
	if (dlg.DoModal() != IDOK)	return;

	OnFree2D(m_outImage, m_outH);
	m_outImage = NULL;
	m_outH = m_outW = 0;

	m_outH = m_inH;
	m_outW = m_inW;
	m_outImage = OnMalloc2D(m_outH, m_outW, 0);

	//마스크 초기화 및 생성
	int val = (int)dlg.m_constant;

	if (val < 1) val = 3;
	if (val % 2 == 0) val += 1;

	OnFreeMask(m_mask, m_ksize);
	m_mask = NULL;
	m_ksize = val;
	m_mask = OnMallocMask(m_ksize);

	for (int y = 0 - int(m_ksize / 2); y < m_ksize / 2 + 1; y++)
		for (int x = 0 - int(m_ksize / 2); x < m_ksize / 2 + 1; x++) {
			m_mask[y + int(m_ksize / 2)][x + int(m_ksize / 2)] = (1.0 / (2.0 * (3.141592))) * pow(2.718281, (0.0 - ((double)x * (double)x / 2 + (double)y * (double)y / 2.0)));
		}


	// 임시 이미지 생성
	double** tempImage;

	int tempH = m_inH + m_ksize - 1;
	tempImage = OnMallocMask(tempH);

	for (int y = 0; y < tempH; y++)
		for (int x = 0; x < tempH; x++)
			tempImage[y][x] = 127;

	for (int y = int(m_ksize / 2); y < m_inH + (int)(m_ksize / 2); y++)
		for (int x = int(m_ksize / 2); x < m_inW + (int)(m_ksize / 2); x++)
			tempImage[y][x] = m_inImage[y - (int)(m_ksize / 2)][x - (int)(m_ksize / 2)];


	// 회선 연산
	double S;
	for (int y = 0; y < m_outH; y++)
		for (int x = 0; x < m_outW; x++) {
			S = 0.0;
			for (int i = 0; i < m_ksize; i++) {
				for (int j = 0; j < m_ksize; j++)
					S += m_mask[i][j] * tempImage[y + i][x + j];
			}
			if (S < 0.0) m_outImage[y][x] = 0;
			else if (S > 255.0) m_outImage[y][x] = 255;
			else m_outImage[y][x] = (unsigned char)S;
		}

	// 임시 그림 free
	OnFreeMask(tempImage, tempH);
}