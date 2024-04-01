#pragma once
#include "afxdialogex.h"


// CConstantDialog 대화 상자

class CConstantDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CConstantDialog)

public:
	CConstantDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CConstantDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Double_Input };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEditConstant();
	double m_constant;
};
