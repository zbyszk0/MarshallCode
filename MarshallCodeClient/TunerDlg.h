#pragma once
#include "afxwin.h"


// CTunerDlg dialog

class CTunerDlg : public CDialog, IMarshallCodeObserver
{
	DECLARE_DYNAMIC(CTunerDlg)

public:
	CTunerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTunerDlg();

	void onTunerEvent(std::string note, unsigned char intonation);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TUNER_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
	CStatic tooLowLabel;
	CStatic tooHighLabel;
	CStatic noteLabel;
};
