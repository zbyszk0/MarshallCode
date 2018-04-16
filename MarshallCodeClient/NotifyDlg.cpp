// NotifyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MarshallCodeClient.h"
#include "NotifyDlg.h"
#include "afxdialogex.h"


// CNotifyDlg dialog

IMPLEMENT_DYNAMIC(CNotifyDlg, CDialog)

CNotifyDlg::CNotifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_NOTIFY, pParent)
{
	auto codeService = MarshallCodeService::Get();
	codeService->addObserver(this);

}

CNotifyDlg::~CNotifyDlg()
{
	auto codeService = MarshallCodeService::Get();
	codeService->removeObserver(this);
}

void CNotifyDlg::onStatusConnected()
{
	this->SendMessage(WM_USER + 1);
};
void CNotifyDlg::onStatusPatchesLoaded() 
{
	this->SendMessage(WM_CLOSE);
};
void CNotifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NOTIFICATION_LABEL, notifcationLabel);
}


BEGIN_MESSAGE_MAP(CNotifyDlg, CDialog)
	ON_WM_TIMER()
	ON_MESSAGE(WM_USER + 1, &CNotifyDlg::OnConnected)
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CNotifyDlg message handlers

afx_msg LRESULT CNotifyDlg::OnConnected(WPARAM w, LPARAM l) 
{
	auto codeService = MarshallCodeService::Get();

	CString loadingMessage;

	loadingMessage.Format(
		TEXT("Connected. Loading patches %d / 100"),
		codeService->getLoadedCount());
	notifcationLabel.SetWindowTextW(loadingMessage);

	return 0;
}

void CNotifyDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 9876)
	{
		auto codeService = MarshallCodeService::Get();

		if (codeService->getLoadedCount() > 0)
		{
			CString loadingMessage;

			loadingMessage.Format(
				TEXT("Connected. Loading patches %d / 100"),
				codeService->getLoadedCount());
			notifcationLabel.SetWindowTextW(loadingMessage);
		}
	}

	__super::OnTimer(nIDEvent);
}


void CNotifyDlg::OnSetFocus(CWnd* pOldWnd)
{
	__super::OnSetFocus(pOldWnd);

}


int CNotifyDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	SetTimer(9876, USER_TIMER_MINIMUM, NULL);

	return 0;
}
