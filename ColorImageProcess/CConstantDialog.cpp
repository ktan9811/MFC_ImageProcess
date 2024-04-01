// CConstantDialog.cpp: 구현 파일
//

#include "pch.h"
#include "ColorImageProcess.h"
#include "afxdialogex.h"
#include "CConstantDialog.h"


// CConstantDialog 대화 상자

IMPLEMENT_DYNAMIC(CConstantDialog, CDialogEx)

CConstantDialog::CConstantDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Double_Input, pParent)
	, m_constant(0)
{

}

CConstantDialog::~CConstantDialog()
{
}

void CConstantDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CONSTANT, m_constant);
}


BEGIN_MESSAGE_MAP(CConstantDialog, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &CConstantDialog::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT_CONSTANT, &CConstantDialog::OnEnChangeEditConstant)
END_MESSAGE_MAP()


// CConstantDialog 메시지 처리기


void CConstantDialog::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CConstantDialog::OnEnChangeEditConstant()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
