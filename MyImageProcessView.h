
// MyImageProcessView.h: CMyImageProcessView 클래스의 인터페이스
//

#pragma once


class CMyImageProcessView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMyImageProcessView() noexcept;
	DECLARE_DYNCREATE(CMyImageProcessView)

// 특성입니다.
public:
	CMyImageProcessDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMyImageProcessView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEqualimage();
	afx_msg void OnAddimage();
	afx_msg void OnInverseImage();
	afx_msg void OnGammaImage();
	afx_msg void OnHistStretch();
	afx_msg void OnHistequal();
	afx_msg void OnRotateImage();
	afx_msg void OnEmbossImage();
	afx_msg void OnAverageBlur();
	afx_msg void OnPrewittYEdge();
	afx_msg void OnPrewittXEdge();
	afx_msg void OnSobelXEdge();
	afx_msg void OnSobelYEdge();
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnLaplacian1DEdge();
	afx_msg void OnLaplacian2Dedge();
	afx_msg void OnBinByVal();
	afx_msg void OnBinByAvg();
	afx_msg void OnBinByMedian();
	afx_msg void OnGaussianblur();
};

#ifndef _DEBUG  // MyImageProcessView.cpp의 디버그 버전
inline CMyImageProcessDoc* CMyImageProcessView::GetDocument() const
   { return reinterpret_cast<CMyImageProcessDoc*>(m_pDocument); }
#endif

