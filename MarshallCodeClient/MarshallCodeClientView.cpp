
// MarshallCodeClientView.cpp : implementation of the CMarshallCodeClientView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MarshallCodeClient.h"
#endif

#include "MarshallCodeClientView.h"
#include "NotifyDlg.h"
#include "TunerDlg.h"
#include "LogDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// control messages
afx_msg void CMarshallCodeClientView::OnDeviceCheckboxChanged(UINT winContolId)
{
	controlMap[winContolId]->Update(winContolId);
}
afx_msg void CMarshallCodeClientView::OnDeviceSelectionChanged(UINT winContolId)
{
	controlMap[winContolId]->Update(winContolId);
}
afx_msg void CMarshallCodeClientView::OnGlobalParameterValueChanged(UINT winContolId, NMHDR *pNMHDR, LRESULT *pResult)
{
	controlMap[winContolId]->Update(winContolId);

	*pResult = 0;
}
afx_msg void CMarshallCodeClientView::OnGlobalParameterTextValueChanged(UINT winContolId) 
{
	// controlMap[winContolId]->Update(winContolId);
}
afx_msg void CMarshallCodeClientView::OnParameterValueChanged(UINT winContolId, NMHDR *pNMHDR, LRESULT *pResult)
{
	controlMap[winContolId]->Update(winContolId);

	*pResult = 0;
}
afx_msg void CMarshallCodeClientView::OnParameterTextValueChanged(UINT winContolId)
{
	// controlMap[winContolId]->Update(winContolId);
}
afx_msg void CMarshallCodeClientView::OnParameterSelectionChanged(UINT winContolId)
{
	controlMap[winContolId]->Update(winContolId);
}

// CMarshallCodeClientView construction/destruction
CMarshallCodeClientView::CMarshallCodeClientView()
	: CFormView(IDD_MARSHALLCODECLIENT_FORM)
{
}
CMarshallCodeClientView::~CMarshallCodeClientView()
{
}
void CMarshallCodeClientView::OnDestroy()
{
	__super::OnDestroy();

	SHUTDOWN_CODE_SERVICE();
}

// IMarshallCodeObserver
void CMarshallCodeClientView::onControlChanged(EControlId controlId, unsigned char value) 
{
	this->SendMessage(WM_USER + 1);
}
void CMarshallCodeClientView::onStatusPatchesLoaded()
{
	this->SendMessage(WM_USER + 2);
}

afx_msg LRESULT CMarshallCodeClientView::OnPatchModified(WPARAM w, LPARAM l)
{
	auto codeService = MarshallCodeService::Get();
	m_pDocument->SetTitle(CString(
		(codeService->getActivePatch().getName() + "***(modified)***").c_str()));

	return 0;
}
afx_msg LRESULT CMarshallCodeClientView::OnPatchesLoaded(WPARAM w, LPARAM l)
{
	auto codeService = MarshallCodeService::Get();
	m_pDocument->SetTitle(CString(codeService->getActivePatch().getName().c_str()));
	InitializeControllers();
	return 0;
}

// Init
void CMarshallCodeClientView::InitializeControllers() {
	DeviceController *deviceController = nullptr;
	ParameterController *parameterController = nullptr;
	PatchController *patchController = new PatchController(presetListbox, presetNameTextbox);

	// global controls
	parameterController = new ParameterController(EControlId::Gain,
		gainTextbox, gainSlider, nullLabel, nullCombobox, controlMap);
	parameterController = new ParameterController(EControlId::Bass,
		bassTextbox, bassSlider, nullLabel, nullCombobox, controlMap);
	parameterController = new ParameterController(EControlId::Middle,
		middleTextbox, middleSlider, nullLabel, nullCombobox, controlMap);
	parameterController = new ParameterController(EControlId::Treble,
		trebleTextbox, trebleSlider, nullLabel, nullCombobox, controlMap);
	parameterController = new ParameterController(EControlId::Volume,
		volumeTextbox, volumeSlider, nullLabel, nullCombobox, controlMap);
	parameterController = new ParameterController(EControlId::AmpGate,
		gateTextbox, gateSlider, nullLabel, nullCombobox, controlMap);

	// Cabinet
	deviceController =
		new DeviceController(EControlId::CabinetState,
			EControlId::CabinetType, cabinetCheckbox, cabinetCombobox, controlMap
		);

	// preamp
	deviceController =
		new DeviceController(EControlId::AmpState, EControlId::AmpType,
			preampCheckbox, preampCombobox, controlMap
		);

	// poweramp
	deviceController =
		new DeviceController(EControlId::PowerAmpState,
			EControlId::PowerAmpType, powerampCheckbox, powerampCombobox, controlMap
		);
	deviceController->AddParameter(
		EControlId::PowerAmpPresence, new ParameterController(
			EControlId::PowerAmpPresence, presenceTextbox, presenceSlider,
			presenceLabel, nullCombobox, controlMap
		)
	);
	deviceController->AddParameter(
		EControlId::PowerAmpResonance, new ParameterController(
			EControlId::PowerAmpResonance, resonanceTextbox, resonanceSlider,
			resonanceLabel, nullCombobox, controlMap
		)
	);

	// reverb
	deviceController =
		new DeviceController(EControlId::ReverbState,
			EControlId::ReverbType, reverbCheckbox, reverbCombobox, controlMap
		);
	deviceController->AddParameter(
		EControlId::ReverbP1, new ParameterController(
			EControlId::ReverbP1, reverbP1Textbox, reverbP1Slider,
			reverbP1Label, nullCombobox, controlMap
		)
	);
	deviceController->AddParameter(
		EControlId::ReverbP2, new ParameterController(
			EControlId::ReverbP2, reverbP2Textbox, reverbP2Slider,
			reverbP2Label, nullCombobox, controlMap
		)
	);
	deviceController->AddParameter(
		EControlId::ReverbP3, new ParameterController(
			EControlId::ReverbP3, reverbP3Textbox, reverbP3Slider,
			reverbP3Label, nullCombobox, controlMap
		)
	);
	deviceController->AddParameter(
		EControlId::ReverbP4, new ParameterController(
			EControlId::ReverbP4, reverbP4Textbox, reverbP4Slider,
			reverbP4Label, nullCombobox, controlMap
		)
	);

	// modulation
	deviceController =
		new DeviceController(EControlId::ModulationState,
			EControlId::ModulationType, modulationCheckbox, modulationCombobox, controlMap
		);
	deviceController->AddParameter(
		EControlId::ModulationP1, new ParameterController(
			EControlId::ModulationP1, modulationP1Textbox, modulationP1Slider,
			modulationP1Label, nullCombobox, controlMap
		)
	);
	deviceController->AddParameter(
		EControlId::ModulationP2, new ParameterController(
			EControlId::ModulationP2, modulationP2Textbox, modulationP2Slider,
			modulationP2Label, nullCombobox, controlMap
		)
	);
	deviceController->AddParameter(
		EControlId::ModulationP3, new ParameterController(
			EControlId::ModulationP3, modulationP3Textbox, modulationP3Slider,
			modulationP3Label, modulationP3Combo, controlMap
		)
	);
	deviceController->AddParameter(
		EControlId::ModulationP4, new ParameterController(
			EControlId::ModulationP4, modulationP4Textbox, modulationP4Slider,
			modulationP4Label, nullCombobox, controlMap
		)
	);

	// pedal
	deviceController =
		new DeviceController(EControlId::PedalState,
			EControlId::PedalType, pedalCheckbox, pedalCombobox, controlMap
		);
	deviceController->AddParameter(
		EControlId::PedalP1, new ParameterController(
			EControlId::PedalP1, pedalP1Textbox, pedalP1Slider,
			pedalP1Label, pedalP1Combo, controlMap
		)
	);
	deviceController->AddParameter(
		EControlId::PedalP2, new ParameterController(
			EControlId::PedalP2, pedalP2Textbox, pedalP2Slider,
			pedalP2Label, nullCombobox, controlMap
		)
	);
	deviceController->AddParameter(
		EControlId::PedalP3, new ParameterController(
			EControlId::PedalP3, pedalP3Textbox, pedalP3Slider,
			pedalP3Label, nullCombobox, controlMap
		)
	);
	deviceController->AddParameter(
		EControlId::PedalP4, new ParameterController(
			EControlId::PedalP4, pedalP4Textbox, pedalP4Slider,
			pedalP4Label, nullCombobox, controlMap
		)
	);

	// delay
	deviceController =
		new DeviceController(EControlId::DelayState,
			EControlId::DelayType, delayCheckbox, delayCombobox, controlMap
		);
	deviceController->AddParameter(
		EControlId::DelayTimeLSB, new ParameterController(
			EControlId::DelayTimeLSB, delayP1Textbox, delayP1Slider,
			delayP1Label, nullCombobox, controlMap
		)
	);
	deviceController->AddParameter(
		EControlId::DelayTimeMSB, new ParameterController(
			EControlId::DelayTimeMSB, delayP1Textbox, delayP1Slider,
			delayP1Label, nullCombobox, controlMap
		)
	);
	deviceController->AddParameter(
		EControlId::DelayP2, new ParameterController(
			EControlId::DelayP2, delayP2Textbox, delayP2Slider,
			delayP2Label, nullCombobox, controlMap
		)
	);
	deviceController->AddParameter(
		EControlId::DelayP3, new ParameterController(
			EControlId::DelayP3, delayP3Textbox, delayP3Slider,
			delayP3Label, delayP3Combo, controlMap
		)
	);
	deviceController->AddParameter(
		EControlId::DelayP4, new ParameterController(
			EControlId::DelayP4, delayP4Textbox, delayP4Slider,
			delayP4Label, nullCombobox, controlMap
		)
	);
}
void CMarshallCodeClientView::InitStaticControls()
{
	MarshallCodeService *codeService = MarshallCodeService::Get();

	static const TCHAR *PowerampNames[] = {
		TEXT("Classic Marshall 100w"),
		TEXT("Vintage Marshall 30w"),
		TEXT("British Class A"),
		TEXT("American Class A/B")
	};

	static const TCHAR *PreampNames[] = {
		TEXT("JTM45"),
		TEXT("Clean DSL"),
		TEXT("Clean American"),
		TEXT("Clean JVM"),
		TEXT("Acoustic"),
		TEXT("Bluesbreaker"),
		TEXT("Plexi"),
		TEXT("Crunch American"),
		TEXT("JCM800"),
		TEXT("50's British"),
		TEXT("OD JVM"),
		TEXT("OD DSL"),
		TEXT("OD American"),
		TEXT("OD Silver Jubilee"),
		TEXT("Natural")
	};

	static const TCHAR *CabinetNames[] = {
		TEXT("1960"),
		TEXT("1960V"),
		TEXT("1960X"),
		TEXT("1960HW"),
		TEXT("1936"),
		TEXT("1936V"),
		TEXT("1912"),
		TEXT("1974CX")
	};

	static const TCHAR *DelayNames[] = {
		TEXT("Studio"),
		TEXT("Vintage"),
		TEXT("Multi"),
		TEXT("Reverse")
	};

	static const TCHAR *ModulationNames[] = {
		TEXT("Chorus"),
		TEXT("Flanger"),
		TEXT("Phaser"),
		TEXT("Tremolo")
	};

	static const TCHAR *PedalNames[] = {
		TEXT("Compressor"),
		TEXT("Distortion"),
		TEXT("Auto Wah"),
		TEXT("Pitch Shifter")
	};

	static const TCHAR *ReverbNames[] = {
		TEXT("Spring"),
		TEXT("Hall"),
		TEXT("Room"),
		TEXT("Stadium")
	};

	codeService->addObserver(this);
	presetNameTextbox.SetLimitText(18);

	bassSlider.SetRange(0, 100);
	middleSlider.SetRange(0, 100);
	trebleSlider.SetRange(0, 100);

	gainSlider.SetRange(0, 100);
	volumeSlider.SetRange(0, 100);
	gateSlider.SetRange(0, 100);

	resonanceSlider.SetRange(0, 100);
	presenceSlider.SetRange(0, 100);

	for (auto PreampName : PreampNames) {
		preampCombobox.AddString(PreampName);
	}

	for (auto PowerampName : PowerampNames) {
		powerampCombobox.AddString(PowerampName);
	}

	for (auto CabinetName : CabinetNames) {
		cabinetCombobox.AddString(CabinetName);
	}

	for (auto ReverbName : ReverbNames) {
		reverbCombobox.AddString(ReverbName);
	}

	for (auto DelayName : DelayNames) {
		delayCombobox.AddString(DelayName);
	}

	for (auto ModulationName : ModulationNames) {
		modulationCombobox.AddString(ModulationName);
	}

	for (auto PedalName : PedalNames) {
		pedalCombobox.AddString(PedalName);
	}

	resonanceLabel.SetWindowTextW(TEXT("Resonance"));
	presenceLabel.SetWindowTextW(TEXT("Presence"));
}
BOOL CMarshallCodeClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CFormView::PreCreateWindow(cs);
}
void CMarshallCodeClientView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	MarshallCodeService::Get()->addObserver(this);
	InitStaticControls();

	CNotifyDlg notify;
	notify.DoModal();
}

// Save / Load
void CMarshallCodeClientView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}
void CMarshallCodeClientView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}
void CMarshallCodeClientView::OnBnClickedSavePatchButton()
{
	MarshallCodeService *codeService = MarshallCodeService::Get();

	CString patchName;

	CStringA patchNameA;
	presetNameTextbox.GetWindowTextW(patchName);
	patchNameA = patchName;

	codeService->getActivePatch().setName(patchNameA.GetString());
	codeService->getActivePatch().commit();
	m_pDocument->SetTitle(CString(
		(codeService->getActivePatch().getName()).c_str()));
}
void CMarshallCodeClientView::OnFileSave()
{
	OnBnClickedSavePatchButton();
}
void CMarshallCodeClientView::OnBnClickedLoadButton()
{
	MarshallCodeService *codeService = MarshallCodeService::Get();
	bool shouldLoad = true;

	if (codeService->getPatchIsDirty()) {
		int result = AfxMessageBox(
			CString(TEXT("Would you like to save your patch changes?")), MB_YESNOCANCEL);

		switch(result) 
		{
			case IDYES: 
			{
				CString patchName;
				
				CStringA patchNameA;
				presetNameTextbox.GetWindowTextW(patchName);
				patchNameA = patchName;

				codeService->getActivePatch().setName(patchNameA.GetString());
				codeService->getActivePatch().commit();
			}break;

			case IDNO: 
			{
			}break; 

			case IDCANCEL:
			default:
				shouldLoad = false;
				break;
		}
	}

	if (shouldLoad) 
	{
		int idx = presetListbox.GetCurSel();
		codeService->switchPatch(idx);

		m_pDocument->SetTitle(CString(codeService->getPatchById(idx).getName().c_str()));
	}
}
void CMarshallCodeClientView::OnLbnDblclkPresetlist()
{
	OnBnClickedLoadButton();
}
void CMarshallCodeClientView::OnContextLoadClicked()
{
	OnBnClickedLoadButton();
}

// Tools
void CMarshallCodeClientView::OnToolsTuner()
{
	CTunerDlg tunerDlg;
	tunerDlg.DoModal();
}
void CMarshallCodeClientView::OnAboutmarshallcodeclientViewlog()
{
	CLogDialog logDlg;
	logDlg.DoModal();
}
void CMarshallCodeClientView::OnHelpViewlog()
{
	CLogDialog logDlg;
	logDlg.DoModal();
}

// "dials"
void CMarshallCodeClientView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	controlMap[pScrollBar->GetDlgCtrlID()]->SoftRefresh();

	__super::OnHScroll(nSBCode, nPos, pScrollBar);
}

// MFC

// CMarshallCodeClientView diagnostics
#ifdef _DEBUG
void CMarshallCodeClientView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMarshallCodeClientView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMarshallCodeClientDoc* CMarshallCodeClientView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMarshallCodeClientDoc)));
	return (CMarshallCodeClientDoc*)m_pDocument;
}
#endif //_DEBUG

 // Message Handling
IMPLEMENT_DYNCREATE(CMarshallCodeClientView, CFormView)
BEGIN_MESSAGE_MAP(CMarshallCodeClientView, CFormView)
	// enable / disable checkboxes
	ON_CONTROL_RANGE(BN_CLICKED, IDC_PREAMP, IDC_PEDAL, 
		&CMarshallCodeClientView::OnDeviceCheckboxChanged)

	// device selection
	ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_PREAMP_COMBO, IDC_PEDAL_COMBO,
		&CMarshallCodeClientView::OnDeviceSelectionChanged)

	// global param sliders
	ON_NOTIFY_RANGE(NM_RELEASEDCAPTURE, IDC_BASS_SLIDER,
		IDC_VOLUME_SLIDER, &CMarshallCodeClientView::OnGlobalParameterValueChanged)

	// global param textboxes
	ON_CONTROL_RANGE(EN_CHANGE, IDC_BASS_TEXTBOX,
		IDC_VOLUME_TEXTBOX, &CMarshallCodeClientView::OnGlobalParameterTextValueChanged)

	// parameter sliders
	ON_NOTIFY_RANGE(NM_RELEASEDCAPTURE, IDC_RESONANCE_SLIDER,
		IDC_PEDAL_P4_SLIDER, &CMarshallCodeClientView::OnParameterValueChanged)

	// parameter text
	ON_CONTROL_RANGE(EN_CHANGE, IDC_GATE_TEXTBOX,
		IDC_PEDAL_P4_TEXTBOX, &CMarshallCodeClientView::OnParameterTextValueChanged)

	// parameter comboboxes
	ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_DELAY_P3_COMBO, IDC_PEDAL_P1_COMBO,
		&CMarshallCodeClientView::OnParameterSelectionChanged)

	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_LBN_DBLCLK(IDC_PRESETLIST, &CMarshallCodeClientView::OnLbnDblclkPresetlist)
	ON_MESSAGE(WM_USER + 1, &CMarshallCodeClientView::OnPatchModified)
	ON_MESSAGE(WM_USER + 2, &CMarshallCodeClientView::OnPatchesLoaded)
	ON_BN_CLICKED(IDC_LOAD_BUTTON, &CMarshallCodeClientView::OnBnClickedLoadButton)
	ON_BN_CLICKED(IDC_SAVE_PATCH_BUTTON, &CMarshallCodeClientView::OnBnClickedSavePatchButton)
	ON_COMMAND(ID_EDIT_CUT, &CMarshallCodeClientView::OnContextLoadClicked)
	ON_COMMAND(ID_TOOLS_TUNER, &CMarshallCodeClientView::OnToolsTuner)
	ON_COMMAND(ID_FILE_SAVE, &CMarshallCodeClientView::OnFileSave)
	ON_COMMAND(ID_ABOUTMARSHALLCODECLIENT_VIEWLOG, &CMarshallCodeClientView::OnAboutmarshallcodeclientViewlog)
	ON_COMMAND(ID_HELP_VIEWLOG, &CMarshallCodeClientView::OnHelpViewlog)
	ON_WM_DESTROY()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

// control linking
void CMarshallCodeClientView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREAMP, preampCheckbox);
	DDX_Control(pDX, IDC_POWERAMP, powerampCheckbox);
	DDX_Control(pDX, IDC_CABINET, cabinetCheckbox);
	DDX_Control(pDX, IDC_REVERB, reverbCheckbox);
	DDX_Control(pDX, IDC_DELAY, delayCheckbox);
	DDX_Control(pDX, IDC_MODULATION, modulationCheckbox);
	DDX_Control(pDX, IDC_PEDAL, pedalCheckbox);

	DDX_Control(pDX, IDC_PRESETLIST, presetListbox);
	DDX_Control(pDX, IDC_PRESET_NAME, presetNameTextbox);

	DDX_Control(pDX, IDC_PREAMP_COMBO, preampCombobox);
	DDX_Control(pDX, IDC_POWERAMP_COMBO, powerampCombobox);
	DDX_Control(pDX, IDC_CABINET_COMBO, cabinetCombobox);
	DDX_Control(pDX, IDC_REVERB_COMBO, reverbCombobox);
	DDX_Control(pDX, IDC_DELAY_COMBO, delayCombobox);
	DDX_Control(pDX, IDC_MODULATION_COMBO, modulationCombobox);
	DDX_Control(pDX, IDC_PEDAL_COMBO, pedalCombobox);
	DDX_Control(pDX, IDC_SAVE_PATCH_BUTTON, savePatchButton);
	DDX_Control(pDX, IDC_BASS_SLIDER, bassSlider);
	DDX_Control(pDX, IDC_MIDDLE_SLIDER, middleSlider);
	DDX_Control(pDX, IDC_TREBLE_SLIDER, trebleSlider);
	DDX_Control(pDX, IDC_GAIN_SLIDER, gainSlider);
	DDX_Control(pDX, IDC_VOLUME_SLIDER, volumeSlider);
	DDX_Control(pDX, IDC_GATE_SLIDER, gateSlider);
	DDX_Control(pDX, IDC_BASS_TEXTBOX, bassTextbox);
	DDX_Control(pDX, IDC_MIDDLE_TEXTBOX, middleTextbox);
	DDX_Control(pDX, IDC_TREBLE_TEXTBOX, trebleTextbox);
	DDX_Control(pDX, IDC_GAIN_TEXTBOX, gainTextbox);
	DDX_Control(pDX, IDC_VOLUME_TEXTBOX, volumeTextbox);
	DDX_Control(pDX, IDC_GATE_TEXTBOX, gateTextbox);
	DDX_Control(pDX, IDC_PRESNCE_LABEL, presenceLabel);
	DDX_Control(pDX, IDC_RESONANCE_LABEL, resonanceLabel);
	DDX_Control(pDX, IDC_PRESENCE_SLIDER, presenceSlider);
	DDX_Control(pDX, IDC_RESONANCE_SLIDER, resonanceSlider);
	DDX_Control(pDX, IDC_PRESENCE_TEXTBOX, presenceTextbox);
	DDX_Control(pDX, IDC_RESONANCE_TEXTBOX, resonanceTextbox);
	DDX_Control(pDX, IDC_PREAMP_LABEL, preampLabel);
	DDX_Control(pDX, IDC_CABINET_LABEL, cabinetLabel);
	DDX_Control(pDX, IDC_POWERAMP_LABEL, powerampLabel);
	DDX_Control(pDX, IDC_REVERB_P1_LABEL, reverbP1Label);
	DDX_Control(pDX, IDC_REVERB_P2_LABEL, reverbP2Label);
	DDX_Control(pDX, IDC_REVERB_P3_LABEL, reverbP3Label);
	DDX_Control(pDX, IDC_REVERB_P4_LABEL, reverbP4Label);
	DDX_Control(pDX, IDC_DELAY_P1_LABEL, delayP1Label);
	DDX_Control(pDX, IDC_DELAY_P2_LABEL, delayP2Label);
	DDX_Control(pDX, IDC_DELAY_P3_LABEL, delayP3Label);
	DDX_Control(pDX, IDC_DELAY_P4_LABEL, delayP4Label);
	DDX_Control(pDX, IDC_MODULATION_P1_LABEL, modulationP1Label);
	DDX_Control(pDX, IDC_MODULATION_P2_LABEL, modulationP2Label);
	DDX_Control(pDX, IDC_MODULATION_P3_LABEL, modulationP3Label);
	DDX_Control(pDX, IDC_MODULATION_P4_LABEL, modulationP4Label);
	DDX_Control(pDX, IDC_PEDAL_P1_LABEL, pedalP1Label);
	DDX_Control(pDX, IDC_PEDAL_P2_LABEL, pedalP2Label);
	DDX_Control(pDX, IDC_PEDAL_P3_LABEL, pedalP3Label);
	DDX_Control(pDX, IDC_PEDAL_P4_LABEL, pedalP4Label);
	DDX_Control(pDX, IDC_REVERB_P1_TEXTBOX, reverbP1Textbox);
	DDX_Control(pDX, IDC_REVERB_P2_TEXTBOX, reverbP2Textbox);
	DDX_Control(pDX, IDC_REVERB_P3_TEXTBOX, reverbP3Textbox);
	DDX_Control(pDX, IDC_REVERB_P4_TEXTBOX, reverbP4Textbox);
	DDX_Control(pDX, IDC_DELAY_P1_TEXTBOX, delayP1Textbox);
	DDX_Control(pDX, IDC_DELAY_P2_TEXTBOX, delayP2Textbox);
	DDX_Control(pDX, IDC_DELAY_P3_TEXTBOX, delayP3Textbox);
	DDX_Control(pDX, IDC_DELAY_P4_TEXTBOX, delayP4Textbox);
	DDX_Control(pDX, IDC_MODULATION_P1_TEXTBOX, modulationP1Textbox);
	DDX_Control(pDX, IDC_MODULATION_P2_TEXTBOX, modulationP2Textbox);
	DDX_Control(pDX, IDC_MODULATION_P3_TEXTBOX, modulationP3Textbox);
	DDX_Control(pDX, IDC_MODULATION_P4_TEXTBOX, modulationP4Textbox);
	DDX_Control(pDX, IDC_PEDAL_P1_TEXTBOX, pedalP1Textbox);
	DDX_Control(pDX, IDC_PEDAL_P2_TEXTBOX, pedalP2Textbox);
	DDX_Control(pDX, IDC_PEDAL_P3_TEXTBOX, pedalP3Textbox);
	DDX_Control(pDX, IDC_PEDAL_P4_TEXTBOX, pedalP4Textbox);
	DDX_Control(pDX, IDC_REVERB_P1_SLIDER, reverbP1Slider);
	DDX_Control(pDX, IDC_REVERB_P2_SLIDER, reverbP2Slider);
	DDX_Control(pDX, IDC_REVERB_P3_SLIDER, reverbP3Slider);
	DDX_Control(pDX, IDC_REVERB_P4_SLIDER, reverbP4Slider);
	DDX_Control(pDX, IDC_DELAY_P1_SLIDER, delayP1Slider);
	DDX_Control(pDX, IDC_DELAY_P2_SLIDER, delayP2Slider);
	DDX_Control(pDX, IDC_DELAY_P3_SLIDER, delayP3Slider);
	DDX_Control(pDX, IDC_DELAY_P4_SLIDER, delayP4Slider);
	DDX_Control(pDX, IDC_MODULATION_P1_SLIDER, modulationP1Slider);
	DDX_Control(pDX, IDC_MODULATION_P2_SLIDER, modulationP2Slider);
	DDX_Control(pDX, IDC_MODULATION_P3_SLIDER, modulationP3Slider);
	DDX_Control(pDX, IDC_MODULATION_P4_SLIDER, modulationP4Slider);
	DDX_Control(pDX, IDC_PEDAL_P1_SLIDER, pedalP1Slider);
	DDX_Control(pDX, IDC_PEDAL_P2_SLIDER, pedalP2Slider);
	DDX_Control(pDX, IDC_PEDAL_P3_SLIDER, pedalP3Slider);
	DDX_Control(pDX, IDC_PEDAL_P4_SLIDER, pedalP4Slider);
	DDX_Control(pDX, IDC_DELAY_P3_COMBO, delayP3Combo);
	DDX_Control(pDX, IDC_MODULATION_P3_COMBO, modulationP3Combo);
	DDX_Control(pDX, IDC_PEDAL_P1_COMBO, pedalP1Combo);
	DDX_Control(pDX, IDC_NULL_COMBOBOX, nullCombobox);
	DDX_Control(pDX, IDC_NULL_LABEL, nullLabel);
}
