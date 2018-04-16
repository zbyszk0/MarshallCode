// TunerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MarshallCodeClient.h"
#include "TunerDlg.h"
#include "afxdialogex.h"


// CTunerDlg dialog

IMPLEMENT_DYNAMIC(CTunerDlg, CDialog)

CTunerDlg::CTunerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TUNER_DLG, pParent)
{
	auto codeService = MarshallCodeService::Get();
	codeService->addObserver(this);
	codeService->turnOnTuner();
}

CTunerDlg::~CTunerDlg()
{
	auto codeService = MarshallCodeService::Get();
	codeService->removeObserver(this);
}

void CTunerDlg::onTunerEvent(std::string note, unsigned char intonation) 
{
	noteLabel.SetWindowTextW(CString(note.c_str()));

	switch (intonation) 
	{
	case 0:
		tooHighLabel.ShowWindow(SW_HIDE);
		tooLowLabel.ShowWindow(SW_SHOW);
		break;

	case 1:
		tooHighLabel.ShowWindow(SW_HIDE);
		tooLowLabel.ShowWindow(SW_HIDE);
		break;

	case 2:
		tooHighLabel.ShowWindow(SW_SHOW);
		tooLowLabel.ShowWindow(SW_HIDE);
		break;

	default:
		break;
	}
}

void CTunerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TOO_LOW_LABEL, tooLowLabel);
	DDX_Control(pDX, IDC_TOO_HIGH_LABEL, tooHighLabel);
	DDX_Control(pDX, IDC_NOTE_NAME_LABEL, noteLabel);
}


BEGIN_MESSAGE_MAP(CTunerDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CTunerDlg::OnBnClickedCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTunerDlg message handlers


void CTunerDlg::OnBnClickedCancel()
{
	auto codeService = MarshallCodeService::Get();
	codeService->turnOffTuner();

	CDialog::OnCancel();
}


void CTunerDlg::OnClose()
{
	auto codeService = MarshallCodeService::Get();
	codeService->turnOffTuner();

	CDialog::OnClose();
}
