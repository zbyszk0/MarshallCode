#pragma once
#include "Device.h"

class Pedal : public Device {
	void resetParams();

	static bool Pedal::isInitialized;

public:
	static const unsigned int PARAM_PEDAL_P1 = 0;
	static const unsigned int PARAM_PEDAL_P2 = 1;
	static const unsigned int PARAM_PEDAL_P3 = 2;
	static const unsigned int PARAM_PEDAL_P4 = 3;

	Pedal();

	~Pedal() {
		for (auto param : params) {
			delete param;
		}
	}

	static void StaticShutdown();

	void serialize(RawPatch &patch) const {
		patch.PedalIndex = getDeviceIndex();
		patch.PedalPower = (getIsPoweredOn() ? 1 : 0);

		patch.PedalParams[PARAM_PEDAL_P1] = params[PARAM_PEDAL_P1]->getValue();
		patch.PedalParams[PARAM_PEDAL_P2] = params[PARAM_PEDAL_P2]->getValue();
		patch.PedalParams[PARAM_PEDAL_P3] = params[PARAM_PEDAL_P3]->getValue();
		patch.PedalParams[PARAM_PEDAL_P4] = params[PARAM_PEDAL_P4]->getValue();
	}

	void deserialize(RawPatch *patch) {
		setDeviceIndex(patch->PedalIndex);
		setIsPoweredOn(patch->PedalPower != 0);

		resetParams();
		params[PARAM_PEDAL_P1]->setValue(patch->PedalParams[PARAM_PEDAL_P1]);
		params[PARAM_PEDAL_P2]->setValue(patch->PedalParams[PARAM_PEDAL_P2]);
		params[PARAM_PEDAL_P3]->setValue(patch->PedalParams[PARAM_PEDAL_P3]);
		params[PARAM_PEDAL_P4]->setValue(patch->PedalParams[PARAM_PEDAL_P4]);

		for (auto param : params) {
			param->addObserver(this);
		}
	}

	virtual void updateControl(EControlId controlId, unsigned char value) {
		switch (controlId) {
		case EControlId::PedalState:
			setIsPoweredOn(value == 1);
			notifyObservers();
			break;

		case EControlId::PedalType:
			setDeviceIndex(value);
			resetParams();
			notifyObservers();
			break;

		case EControlId::PedalP1:
			params[PARAM_PEDAL_P1]->setValue(value);
			break;

		case EControlId::PedalP2:
			params[PARAM_PEDAL_P2]->setValue(value);
			break;

		case EControlId::PedalP3:
			params[PARAM_PEDAL_P3]->setValue(value);
			break;

		case EControlId::PedalP4:
			params[PARAM_PEDAL_P4]->setValue(value);
			break;

		default:
			break;
		}
	}
};
