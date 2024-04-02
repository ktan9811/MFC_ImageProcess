
// ColorImageProcessDoc.h: CColorImageProcessDoc 클래스의 인터페이스
//


#pragma once


class CColorImageProcessDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CColorImageProcessDoc() noexcept;
	DECLARE_DYNCREATE(CColorImageProcessDoc)

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
	virtual ~CColorImageProcessDoc();
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
	unsigned char** m_inImageR = NULL;
	unsigned char** m_inImageG = NULL;
	unsigned char** m_inImageB = NULL;
	int m_inH = 0;
	int m_inW = 0;

	unsigned char** m_outImageR = NULL;
	unsigned char** m_outImageG = NULL;
	unsigned char** m_outImageB = NULL;
	int m_outH = 0;
	int m_outW = 0;

	unsigned char** m_tempImageR = NULL;
	unsigned char** m_tempImageG = NULL;
	unsigned char** m_tempImageB = NULL;

	int m_tempH = 0;
	int m_tempW = 0;

	double** m_mask = NULL;
	int m_ksize = 0;

	double** m_doubleImageR = NULL;
	double** m_doubleImageG = NULL;
	double** m_doubleImageB = NULL;
	int m_dimH = 0;
	int m_dimW = 0;

	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	void OnFree2D(unsigned char** memory, int h);
	unsigned char** OnMalloc2D(int h, int w);
	virtual void OnCloseDocument();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	void OnEqualmage();
	void OnFreeOutImage();
	void OnGrayScale();
	void OnMallocOutImage();
	double* RGB2HSI(int R, int G, int B);
	unsigned char* HSI2RGB(double H, double S, double I);
	void OnChangeSaturation();
	void OnAddImage();
	void OnInverseImage();
	void OnImageGamma();
	int OnChangeIntensity();
	void OnChangeHue();
	void OnRotateByDegree();
	void OnZoomIn();
	void OnZoomOut();
	void OnCyberpunk();

	void OnFreeTempImage();
	void OnMallocTempImage();
	void OnFreeMask();
	double** OnMalloc2DMask(int ksize);
	void OnAvgBlur();
	void OnSetAvgMask(int ksize);
	void OnMallocDoubleImage();
	void OnFreeDoubleImage();
	void OnGaussianBlur();
	void OnSetGaussianMask(int ksize);
	void OnEmbossImage();
	void OnPrewittXedge();
	void OnPrewittYedge();
	int OnSobleXedge();
	void OnSobleYedge();
	void OnLaplacian1dEdge();
	void OnLaplacian2dEdge();
	void OnLaplacianOfGaussian();
	void OnHistStrech();
	void OnHistEqual();
};
