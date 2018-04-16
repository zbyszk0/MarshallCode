// LogDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MarshallCodeClient.h"
#include "LogDialog.h"
#include "afxdialogex.h"


// CLogDialog dialog

IMPLEMENT_DYNAMIC(CLogDialog, CDialog)

CLogDialog::CLogDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_LOG_DLG, pParent)
{

}

CLogDialog::~CLogDialog()
{
}

void CLogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOG_TEXTBOX, logTextbox);
}


BEGIN_MESSAGE_MAP(CLogDialog, CDialog)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CLogDialog message handlers


int CLogDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}


void CLogDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	std::ifstream file("log.txt");
	std::string log((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());
	logTextbox.SetWindowTextW(CString(log.c_str()));
}
