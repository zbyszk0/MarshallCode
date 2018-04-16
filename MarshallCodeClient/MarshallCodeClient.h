
// MarshallCodeClient.h : main header file for the MarshallCodeClient application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMarshallCodeClientApp:
// See MarshallCodeClient.cpp for the implementation of this class
//

class CMarshallCodeClientApp : public CWinAppEx
{
public:
	CMarshallCodeClientApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMarshallCodeClientApp theApp;
