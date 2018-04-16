
// MarshallCodeClientView.h : interface of the CMarshallCodeClientView class
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MarshallCodeClientDoc.h"
#include "MarshallViewControllers.h"

class CMarshallCodeClientView : public CFormView, public IMarshallCodeObserver
{
	ControllerMap controlMap;

protected: // create from serialization only
	CMarshallCodeClientView();
	DECLARE_DYNCREATE(CMarshallCodeClientView)

protected:
	// Marshall Code UI init
	void InitStaticControls();

#pragma region MFC Init
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
#pragma endregion 

public:
	virtual ~CMarshallCodeClientView();
	DECLARE_MESSAGE_MAP()

#pragma region Document
	CMarshallCodeClientDoc* GetDocument() const;
	LRESULT OnPatchModified(WPARAM w, LPARAM l);
	LRESULT OnPatchesLoaded(WPARAM w, LPARAM l);
	void InitializeControllers();
#pragma endregion

#pragma region IMarshallCodeObserver
	void onControlChanged(EControlId controlId, unsigned char value);
	void onStatusPatchesLoaded();
#pragma endregion 

#pragma region Update UI
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
#pragma endregion 

#pragma region Control Handlers
	afx_msg void OnDeviceCheckboxChanged(UINT winContolId);
	afx_msg void OnDeviceSelectionChanged(UINT winContolId);
	afx_msg void OnGlobalParameterValueChanged(UINT winContolId, NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGlobalParameterTextValueChanged(UINT winContolId);
	afx_msg void OnParameterValueChanged(UINT winContolId, NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnParameterTextValueChanged(UINT winContolId);
	afx_msg void OnParameterSelectionChanged(UINT winContolId);
	afx_msg void OnLbnDblclkPresetlist();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
#pragma endregion 

#pragma region Menus
	afx_msg void OnToolsTuner();
	afx_msg void OnFileSave();
	afx_msg void OnAboutmarshallcodeclientViewlog();
	afx_msg void OnHelpViewlog();

	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
#pragma endregion 

#pragma region Save/Load
	afx_msg void OnBnClickedLoadButton();
	afx_msg void OnBnClickedSavePatchButton();
	afx_msg void OnContextLoadClicked();
#pragma endregion 

#pragma region Lifecycle
	afx_msg void OnDestroy();
#pragma endregion

public:
#pragma region Controls
	CButton preampCheckbox;
	CButton powerampCheckbox;
	CButton cabinetCheckbox;
	CButton reverbCheckbox;
	CButton delayCheckbox;
	CButton modulationCheckbox;
	CButton pedalCheckbox;
	CListBox presetListbox;
	CEdit presetNameTextbox;
	CComboBox preampCombobox;
	CComboBox powerampCombobox;
	CComboBox cabinetCombobox;
	CComboBox reverbCombobox;
	CComboBox delayCombobox;
	CComboBox modulationCombobox;
	CComboBox pedalCombobox;
	CButton savePatchButton;
	CSliderCtrl bassSlider;
	CSliderCtrl middleSlider;
	CSliderCtrl trebleSlider;
	CSliderCtrl gainSlider;
	CSliderCtrl volumeSlider;
	CSliderCtrl gateSlider;
	CEdit bassTextbox;
	CEdit middleTextbox;
	CEdit trebleTextbox;
	CEdit gainTextbox;
	CEdit volumeTextbox;
	CEdit gateTextbox;
	CStatic presenceLabel;
	CStatic resonanceLabel;
	CSliderCtrl presenceSlider;
	CSliderCtrl resonanceSlider;
	CEdit presenceTextbox;
	CEdit resonanceTextbox;
	CStatic preampLabel;
	CStatic cabinetLabel;
	CStatic powerampLabel;
	CStatic reverbP1Label;
	CStatic reverbP2Label;
	CStatic reverbP3Label;
	CStatic reverbP4Label;
	CStatic delayP1Label;
	CStatic delayP2Label;
	CStatic delayP3Label;
	CStatic delayP4Label;
	CStatic modulationP1Label;
	CStatic modulationP2Label;
	CStatic modulationP3Label;
	CStatic modulationP4Label;
	CStatic pedalP1Label;
	CStatic pedalP2Label;
	CStatic pedalP3Label;
	CStatic pedalP4Label;
	CEdit reverbP1Textbox;
	CEdit reverbP2Textbox;
	CEdit reverbP3Textbox;
	CEdit reverbP4Textbox;
	CEdit delayP1Textbox;
	CEdit delayP2Textbox;
	CEdit delayP3Textbox;
	CEdit delayP4Textbox;
	CEdit modulationP1Textbox;
	CEdit modulationP2Textbox;
	CEdit modulationP3Textbox;
	CEdit modulationP4Textbox;
	CEdit pedalP1Textbox;
	CEdit pedalP2Textbox;
	CEdit pedalP3Textbox;
	CEdit pedalP4Textbox;
	CSliderCtrl reverbP1Slider;
	CSliderCtrl reverbP2Slider;
	CSliderCtrl reverbP3Slider;
	CSliderCtrl reverbP4Slider;
	CSliderCtrl delayP1Slider;
	CSliderCtrl delayP2Slider;
	CSliderCtrl delayP3Slider;
	CSliderCtrl delayP4Slider;
	CSliderCtrl modulationP1Slider;
	CSliderCtrl modulationP2Slider;
	CSliderCtrl modulationP3Slider;
	CSliderCtrl modulationP4Slider;
	CSliderCtrl pedalP1Slider;
	CSliderCtrl pedalP2Slider;
	CSliderCtrl pedalP3Slider;
	CSliderCtrl pedalP4Slider;
	CComboBox delayP3Combo;
	CComboBox modulationP3Combo;
	CComboBox pedalP1Combo;
#pragma endregion

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MARSHALLCODECLIENT_FORM };
#endif
	CComboBox nullCombobox;
	CStatic nullLabel;
};

#ifndef _DEBUG  // debug version in MarshallCodeClientView.cpp
inline CMarshallCodeClientDoc* CMarshallCodeClientView::GetDocument() const
   { return reinterpret_cast<CMarshallCodeClientDoc*>(m_pDocument); }
#endif

