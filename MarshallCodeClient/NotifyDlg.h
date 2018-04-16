#pragma once
#include "afxwin.h"


// CNotifyDlg dialog

class CNotifyDlg : public CDialog, IMarshallCodeObserver
{
	DECLARE_DYNAMIC(CNotifyDlg)

public:
	CNotifyDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNotifyDlg();
	void onStatusConnected();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NOTIFY };
#endif

protected:
	void onStatusPatchesLoaded();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic notifcationLabel;
	afx_msg
		LRESULT OnConnected(WPARAM w, LPARAM l);
	void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
