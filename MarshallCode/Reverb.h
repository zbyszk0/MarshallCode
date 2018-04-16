#pragma once
#include "Device.h"

class Reverb : public Device {
	void resetParams();

	static bool isInitialized;

public:
	static const unsigned int PARAM_REVERB_P1 = 0;
	static const unsigned int PARAM_REVERB_P2 = 1;
	static const unsigned int PARAM_REVERB_P3 = 2;
	static const unsigned int PARAM_REVERB_P4 = 3;

	Reverb();

	~Reverb() {
		for (auto param : params) {
			delete param;
		}
	}

	static void StaticShutdown();

	void serialize(RawPatch &patch) const {
		patch.ReverbIndex = getDeviceIndex();
		patch.ReverbPower = (getIsPoweredOn() ? 1 : 0);

		patch.ReverbParams[PARAM_REVERB_P1] = params[PARAM_REVERB_P1]->getValue();
		patch.ReverbParams[PARAM_REVERB_P2] = params[PARAM_REVERB_P2]->getValue();
		patch.ReverbParams[PARAM_REVERB_P3] = params[PARAM_REVERB_P3]->getValue();
		patch.ReverbParams[PARAM_REVERB_P4] = params[PARAM_REVERB_P4]->getValue();
	}

	void deserialize(RawPatch *patch) {
		setDeviceIndex(patch->ReverbIndex);
		setIsPoweredOn(patch->ReverbPower != 0);

		resetParams();
		params[PARAM_REVERB_P1]->setValue(patch->ReverbParams[PARAM_REVERB_P1]);
		params[PARAM_REVERB_P2]->setValue(patch->ReverbParams[PARAM_REVERB_P2]);
		params[PARAM_REVERB_P3]->setValue(patch->ReverbParams[PARAM_REVERB_P3]);
		params[PARAM_REVERB_P4]->setValue(patch->ReverbParams[PARAM_REVERB_P4]);

		for (auto param : params) {
			param->addObserver(this);
		}
	}

	virtual void updateControl(EControlId controlId, unsigned char value) {
		switch (controlId) {
		case EControlId::ReverbState:
			setIsPoweredOn(value == 1);
			notifyObservers();
			break;

		case EControlId::ReverbType:
			setDeviceIndex(value);
			resetParams();
			notifyObservers();
			break;

		case EControlId::ReverbP1:
			params[PARAM_REVERB_P1]->setValue(value);
			break;

		case EControlId::ReverbP2:
			params[PARAM_REVERB_P2]->setValue(value);
			break;

		case EControlId::ReverbP3:
			params[PARAM_REVERB_P3]->setValue(value);
			break;

		case EControlId::ReverbP4:
			params[PARAM_REVERB_P4]->setValue(value);
			break;

		default:
			break;
		}
	}
};
