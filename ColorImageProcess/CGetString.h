#pragma once
#include "afxdialogex.h"


// CGetString 대화 상자

class CGetString : public CDialogEx
{
	DECLARE_DYNAMIC(CGetString)

public:
	CGetString(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CGetString();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GET_STRING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEditString();
	CString m_string;
};
