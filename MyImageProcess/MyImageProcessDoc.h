
// MyImageProcessDoc.h: CMyImageProcessDoc 클래스의 인터페이스
//


#pragma once


class CMyImageProcessDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CMyImageProcessDoc() noexcept;
	DECLARE_DYNCREATE(CMyImageProcessDoc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CMyImageProcessDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	unsigned char** m_inImage = nullptr;
	unsigned char** m_outImage = nullptr;

	int m_inH;
	int m_inW;

	int m_outH;
	int m_outW;

	double** m_mask = nullptr;
	int m_ksize = 0;

	unsigned char** OnMalloc2D(int H, int W, int initVal);
	void OnFree2D(unsigned char** memory, int H);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	void OnEqualImage();
	void OnAddimage();
	void OnInverseImage();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	afx_msg void OnGammaImage();
	void HistStretch();
	void OnHistEqual();
	void OnRotateImage();

	double** OnMallocMask(int K);
	void OnFreeMask(double** mask, int K);
	void OnEmbossImage();
	void OnAverageBlur();
	afx_msg void OnPrewittXEdge();
	void OnPrewittYEdge();
	void SobelXEdge();
	void OnSobelYEdge();
	int OnZoomIn();
	int OnZoomOut();
	void OnLaplacian1DEdge();
	void OnLaplacian2DEdge();
	void OnBinByVal();
	void OnBinByAvg();
	void OnBinByMedian();
	void OnGaussianBlur();
};
