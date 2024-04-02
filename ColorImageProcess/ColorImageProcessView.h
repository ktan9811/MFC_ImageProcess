
// ColorImageProcessView.h: CColorImageProcessView 클래스의 인터페이스
//

#pragma once


class CColorImageProcessView : public CView
{
protected: // serialization에서만 만들어집니다.
	CColorImageProcessView() noexcept;
	DECLARE_DYNCREATE(CColorImageProcessView)

// 특성입니다.
public:
	CColorImageProcessDoc* GetDocument() const;

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
	virtual ~CColorImageProcessView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEqualmage();
	afx_msg void OnGrayScale();
	afx_msg void OnChangeHue();
	afx_msg void OnChangeSaturation();
	afx_msg void OnChangeIntensity();
	afx_msg void OnAddImage();
	afx_msg void OnInverseImage();
	afx_msg void OnImageGamma();
	afx_msg void OnRotateByDegree();
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnCyberpunk();
	afx_msg void OnAvgBlur();
	afx_msg void OnGaussianblur();
	afx_msg void OnEmbossImage();
	afx_msg void OnPrewittXedge();
	afx_msg void OnPrewittYedge();
	afx_msg void OnSobleYedge();
	afx_msg void OnSobleXedge();
	afx_msg void OnLaplacian1dEdge();
	afx_msg void OnLaplacian2dEdge();
	afx_msg void OnLaplacianOfGaussian();
	afx_msg void OnHistStrech();
	afx_msg void OnHistEqual();
};

#ifndef _DEBUG  // ColorImageProcessView.cpp의 디버그 버전
inline CColorImageProcessDoc* CColorImageProcessView::GetDocument() const
   { return reinterpret_cast<CColorImageProcessDoc*>(m_pDocument); }
#endif

