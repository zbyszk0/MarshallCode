#pragma once
#include "Device.h"

class Preamp : public Device {
	void resetParams();

public:
	static const unsigned int PARAM_GATE = 0;

	Preamp();

	~Preamp() {
		for (auto param : params) {
			delete param;
		}
	}

	void serialize(RawPatch &patch) const {
		patch.PreampIndex = getDeviceIndex();
		patch.PreampPower = (getIsPoweredOn() ? 1 : 0);

		patch.PreampGate = params[PARAM_GATE]->getValue();
	}

	void deserialize(RawPatch *patch) {
		setDeviceIndex(patch->PreampIndex);
		setIsPoweredOn(patch->PreampPower != 0);

		resetParams();
		params[PARAM_GATE]->setValue(patch->PreampGate);

		for (auto param : params) {
			param->addObserver(this);
		}
	}

	virtual void updateControl(EControlId controlId, unsigned char value) {
		switch (controlId) {
		case EControlId::AmpState:
			setIsPoweredOn(value == 1);
			notifyObservers();
			break;

		case EControlId::AmpType:
			setDeviceIndex(value);
			notifyObservers();
			break;

		case EControlId::AmpGate:
			params[PARAM_GATE]->setValue(value);
			break;

		default:
			break;
		}
	}
};
