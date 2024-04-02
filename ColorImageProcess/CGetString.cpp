// CGetString.cpp: 구현 파일
//

#include "pch.h"
#include "ColorImageProcess.h"
#include "afxdialogex.h"
#include "CGetString.h"


// CGetString 대화 상자

IMPLEMENT_DYNAMIC(CGetString, CDialogEx)

CGetString::CGetString(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GET_STRING, pParent)
	, m_string(_T(""))
{

}

CGetString::~CGetString()
{
}

void CGetString::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STRING, m_string);
}


BEGIN_MESSAGE_MAP(CGetString, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &CGetString::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT_STRING, &CGetString::OnEnChangeEditString)
END_MESSAGE_MAP()


// CGetString 메시지 처리기


void CGetString::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CGetString::OnEnChangeEditString()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
