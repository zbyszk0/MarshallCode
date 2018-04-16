#pragma once
#include "Device.h"

class Poweramp : public Device {
	void resetParams();

public:
	static const unsigned int PARAM_PRESENCE = 0;
	static const unsigned int PARAM_RESONANCE = 1;

	Poweramp();

	~Poweramp() {
		for (auto param : params) {
			delete param;
		}
	}

	void serialize(RawPatch &patch) const {
		patch.PowerAmpIndex = getDeviceIndex();
		patch.PowerAmpPower = (getIsPoweredOn() ? 1 : 0);

		patch.PowerAmpPresence = params[PARAM_PRESENCE]->getValue();
		patch.PowerAmpResonance = params[PARAM_RESONANCE]->getValue();
	}

	void deserialize(RawPatch *patch) {
		setDeviceIndex(patch->PowerAmpIndex);
		setIsPoweredOn(patch->PowerAmpPower != 0);

		resetParams();
		params[PARAM_PRESENCE]->setValue(patch->PowerAmpPresence);
		params[PARAM_RESONANCE]->setValue(patch->PowerAmpResonance);

		for (auto &param : params) {
			param->addObserver(this);
		}
	}

	virtual void updateControl(EControlId controlId, unsigned char value) {
		switch (controlId) {
		case EControlId::PowerAmpState:
			setIsPoweredOn(value == 1);
			notifyObservers();
			break;

		case EControlId::PowerAmpType:
			setDeviceIndex(value);
			notifyObservers();
			break;

		case EControlId::PowerAmpPresence:
			params[PARAM_PRESENCE]->setValue(value);
			break;

		case EControlId::PowerAmpResonance:
			params[PARAM_RESONANCE]->setValue(value);
			break;

		default:
			break;
		}
	}
};
