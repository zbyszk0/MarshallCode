#pragma once

/*
* Interface for UI Targets (Objects that are affected by UI elements)
*/
class IUiController : public IMarshallCodeObserver {
	EControlId INVALID_CONTROL = (EControlId)0xff;

protected:
	MarshallCodeService *codeService;
	EControlId controlId, controlId_devicelist, controlId_checkbox;

public:
	IUiController() : codeService(MarshallCodeService::Get()) {
		codeService->addObserver(this);

		controlId = INVALID_CONTROL;
		controlId_devicelist = INVALID_CONTROL;
		controlId_checkbox = INVALID_CONTROL;
	}

	// IMarshallCodeObserver
	void onStatusPatchesLoaded() override {
		this->Reset();
	}
	void onActivePatchSwitched(unsigned char newActivePatch) override {
		this->Reset();
	}
	void onControlChanged(EControlId controlId, unsigned char value) override {
		if (this->controlId == controlId || 
			this->controlId_devicelist == controlId ||
			this->controlId_checkbox == controlId) { 
			this->RefreshUI(controlId);
		}
	}

	virtual void SoftRefresh() {};
	virtual void RefreshUI(EControlId controlId) = 0;
	virtual void Update(UINT winControlId) = 0;
	virtual void Reset() = 0;
};

/*
* Mapping for windows control IDs -> UI Targets
*/
typedef CMap<unsigned int, unsigned int, IUiController *, IUiController *> ControllerMap;

/*
* Controllers for device, parameter and patch
*/
class ParameterController : public IUiController {
	CEdit &textbox;
	CSliderCtrl &slider;
	CStatic &label;
	CComboBox &combobox;

	void Enable() {
		slider.EnableWindow();
		combobox.EnableWindow();
		label.EnableWindow();
	}
	void Disable() {
		slider.EnableWindow(false);
		combobox.EnableWindow(false);
		label.EnableWindow(false);
	}
	void RefreshGlobalParameter(EControlId controlId) {
		CString textValue = TEXT("ERROR");
		unsigned int value = 0;

		switch (controlId) {
		case EControlId::Treble:
			value = codeService->getActivePatch().getTreble();
			break;

		case EControlId::Middle:
			value = codeService->getActivePatch().getMiddle();
			break;

		case EControlId::Bass:
			value = codeService->getActivePatch().getBass();
			break;

		case EControlId::Gain:
			value = codeService->getActivePatch().getGain();
			break;

		case EControlId::Volume:
			value = codeService->getActivePatch().getVolume();
			break;

		case EControlId::AmpGate:
			value = codeService->getActivePatch().getPreamp()
				.getParameter(Preamp::PARAM_GATE)->getValue();
			break;
		}

		textValue.Format(TEXT("%.1f"), value / 10.f);
		slider.SetRange(0, 100);
		slider.SetPos(value);
		textbox.SetWindowTextW(textValue);
	}
	void RefreshDeviceParameter(EControlId controlId) {
		CString textValue = TEXT("ERROR");
		unsigned int value = 0;

		auto param = codeService->getActivePatch().getParameterByControlId(controlId);
		auto device = codeService->getActivePatch().getDeviceByControlId(controlId);

		if (param->isDial()) {
			combobox.ShowWindow(SW_HIDE);
			slider.ShowWindow(SW_SHOW);

			textValue.Format(TEXT("%.1f"), param->getFriendlyValue());
			slider.SetRange(param->getMin(), param->getMax());

			textbox.SetWindowTextW(textValue);
			slider.SetPos(param->getValue());
		}
		else if (param->isSelector()) {
			slider.ShowWindow(SW_HIDE);
			combobox.ShowWindow(SW_SHOW);

			UpdateOptions(controlId);

			textValue = CString(param->getSelection().c_str());
			textbox.SetWindowTextW(textValue);
			combobox.SetCurSel(param->getValue());
		}

		if (device->getIsPoweredOn()) {
			Enable();
		}
		else {
			Disable();
		}
	}
	void UpdateOptions(EControlId controlId) {
		auto param = codeService->getActivePatch().getParameterByControlId(controlId);

		if (param && param->isSelector()) {
			combobox.ResetContent();

			for (auto option : param->getOptions()) {
				combobox.AddString(CString(option.c_str()));
			}
		}
	}
	void UpdateLabel(EControlId controlId) {
		if (!IsGlobalParameter(controlId)) {
			auto labelText = codeService->getActivePatch()
				.getParameterByControlId(controlId)->getLabel();

			label.SetWindowTextW(CString(labelText.c_str()));
		}
	}

public:


	ParameterController(EControlId controlId, CEdit &textbox, CSliderCtrl &slider,
		CStatic &label, CComboBox &combobox, ControllerMap &controlMap) :
		textbox(textbox), slider(slider), label(label), combobox(combobox) {
		this->controlId = controlId;

		controlMap.SetAt(textbox.GetDlgCtrlID(), this);
		controlMap.SetAt(slider.GetDlgCtrlID(), this);
		controlMap.SetAt(label.GetDlgCtrlID(), this);
		controlMap.SetAt(combobox.GetDlgCtrlID(), this);

		if (!IsGlobalParameter(controlId)) {
			slider.EnableWindow(false);
			label.EnableWindow(false);
			combobox.EnableWindow(false);
		}

		UpdateOptions(controlId);
		UpdateLabel(controlId);

		textbox.EnableWindow(false);
	}
	~ParameterController() {

	}

	void SoftRefresh() {
		CString textValue = TEXT("ERROR");
		unsigned int value = 0;

		if (IsGlobalParameter(controlId)) {
			value = slider.GetPos();
			textValue.Format(TEXT("%.1f"), value / 10.f);
		}
		else {
			auto param = codeService->getActivePatch().getParameterByControlId(controlId);
			value = slider.GetPos();
			textValue.Format(TEXT("%.1f"), value * param->getStepSize());
		}
		textbox.SetWindowTextW(textValue);
	}
	void RefreshUI(EControlId controlId) {

		if (IsGlobalParameter(controlId)) {
			RefreshGlobalParameter(controlId);
		}
		else {
			RefreshDeviceParameter(controlId);
		}
	}
	void Update(UINT winControlId) {
		unsigned char value = 0;

		if (winControlId == textbox.GetDlgCtrlID()) {
			CString windowText;
			textbox.GetWindowTextW(windowText);
			value = (_wtof(windowText.GetString()) * 100);
		}
		else if (winControlId == slider.GetDlgCtrlID()) {
			value = slider.GetPos();
		}
		else if (winControlId == combobox.GetDlgCtrlID()) {
			value = combobox.GetCurSel();
		}

		codeService->changeControl(controlId, value);
	}
	void Reset() {
		RefreshUI(controlId);
	}

	static bool IsGlobalParameter(EControlId controlId) {
		return (controlId == EControlId::Treble ||
			controlId == EControlId::Middle ||
			controlId == EControlId::Gain ||
			controlId == EControlId::Volume ||
			controlId == EControlId::Bass ||
			controlId == EControlId::AmpGate);
	}
};

class DeviceController : public IUiController {
	CButton &checkbox;
	CComboBox &devicelist;

	CMap<EControlId, EControlId, ParameterController *, ParameterController *> parameters;

	void TurnOn() {
		devicelist.EnableWindow();

		POSITION p = parameters.GetStartPosition();

		while (p != NULL) {
			EControlId id;
			ParameterController *param;
			parameters.GetNextAssoc(p, id, param);

			param->Reset();
		}
	}
	void TurnOff() {
		devicelist.EnableWindow(false);

		POSITION p = parameters.GetStartPosition();

		while (p != NULL) {
			EControlId id;
			ParameterController *param;
			parameters.GetNextAssoc(p, id, param);

			param->Reset();
		}
	}

public:
	DeviceController(EControlId controlId_checkbox, EControlId controlId_devicelist,
		CButton &checkbox, CComboBox &devicelist, ControllerMap &controlMap)
		: checkbox(checkbox), devicelist(devicelist) {

		this->controlId_devicelist = controlId_devicelist;
		this->controlId_checkbox = controlId_checkbox;

		controlMap.SetAt(devicelist.GetDlgCtrlID(), this);
		controlMap.SetAt(checkbox.GetDlgCtrlID(), this);

		devicelist.EnableWindow(false);
	}
	~DeviceController() {}

	void RefreshUI(EControlId controlId) override {
		if (controlId == this->controlId_checkbox) {
			unsigned char value = codeService->getActivePatch()
				.getDeviceByControlId(controlId_checkbox)->getIsPoweredOn();

			checkbox.SetCheck(value);

			if (value) {
				TurnOn();
			}
			else {
				TurnOff();
			}
		}
		else if (controlId == this->controlId_devicelist) {
			unsigned char value = codeService->getActivePatch()
				.getDeviceByControlId(controlId_devicelist)->getDeviceIndex();

			devicelist.SetCurSel(value);
			POSITION p = parameters.GetStartPosition();

			while (p != NULL) {
				EControlId id;
				ParameterController *param;
				parameters.GetNextAssoc(p, id, param);

				param->Reset();
			}
		}
	}
	void Reset() override {
		RefreshUI(controlId_checkbox);
		RefreshUI(controlId_devicelist);
	}
	void Update(UINT winControlId) override {
		if (winControlId == checkbox.GetDlgCtrlID()) {
			codeService->changeControl(controlId_checkbox, checkbox.GetCheck());
		}
		else if (winControlId == devicelist.GetDlgCtrlID()) {
			codeService->changeControl(controlId_devicelist, devicelist.GetCurSel());
		}
	}

	void AddParameter(const EControlId &id, ParameterController *param) {
		parameters.SetAt(id, param);
	}

	static bool IsDeviceControl(EControlId controlId) {
		return (controlId == EControlId::AmpState ||
			controlId == EControlId::CabinetState ||
			controlId == EControlId::PowerAmpState ||
			controlId == EControlId::DelayState ||
			controlId == EControlId::ReverbState ||
			controlId == EControlId::PedalState ||
			controlId == EControlId::ModulationState);
	}
	static bool IsDeviceTypeControl(EControlId controlId) {
		return (controlId == EControlId::AmpType ||
			controlId == EControlId::CabinetType ||
			controlId == EControlId::PowerAmpType ||
			controlId == EControlId::DelayType ||
			controlId == EControlId::ReverbType ||
			controlId == EControlId::PedalType ||
			controlId == EControlId::ModulationType);
	}
};

class PatchController : public IUiController {
	unsigned char patchId;
	CEdit &nameTextbox;
	CListBox &presetListbox;
	CStringA ansiName;

public:
	PatchController(CListBox &presetListbox, CEdit &nameTextbox) :
		presetListbox(presetListbox), nameTextbox(nameTextbox) {
	}
	~PatchController() {}

	void RefreshUI(EControlId controlId) override {
		patchId = codeService->getActivePatch().getNumber();

		presetListbox.ResetContent();

		for (int i = 0; i < 100; ++i) {
			presetListbox.AddString(CString(codeService->getPatchById(i).getName().c_str()));
		}

		CString name = CString(codeService->getActivePatch().getName().c_str());
		ansiName = name;

		nameTextbox.SetWindowTextW(name);
		presetListbox.SetCurSel(patchId);
	}
	void Update(UINT winControlId) override {
		CString name;
		nameTextbox.GetWindowTextW(name);
		ansiName = name;
		codeService->getActivePatch().setName(ansiName);
	}
	void Reset() override {
		RefreshUI(EControlId::Bass);
	}
};
