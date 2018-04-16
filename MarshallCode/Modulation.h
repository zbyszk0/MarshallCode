#pragma once
#include "Device.h"

class Modulation : public Device {
	void resetParams();

	static bool isInitialized;

public:
	static const unsigned int PARAM_MOD_P1 = 0;
	static const unsigned int PARAM_MOD_P2 = 1;
	static const unsigned int PARAM_MOD_P3 = 2;
	static const unsigned int PARAM_MOD_P4 = 3;

	Modulation();

	~Modulation() {
		for (auto param : params) {
			delete param;
		}
	}

	static void StaticShutdown();

	void serialize(RawPatch &patch) const {
		patch.ModulationIndex = getDeviceIndex();
		patch.ModulationPower = (getIsPoweredOn() ? 1 : 0);

		patch.ModulationParams[PARAM_MOD_P1] = params[PARAM_MOD_P1]->getValue();
		patch.ModulationParams[PARAM_MOD_P2] = params[PARAM_MOD_P2]->getValue();
		patch.ModulationParams[PARAM_MOD_P3] = params[PARAM_MOD_P3]->getValue();
		patch.ModulationParams[PARAM_MOD_P4] = params[PARAM_MOD_P4]->getValue();
	}

	void deserialize(RawPatch *patch) {
		setDeviceIndex(patch->ModulationIndex);
		setIsPoweredOn(patch->ModulationPower != 0);

		resetParams();
		params[PARAM_MOD_P1]->setValue(patch->ModulationParams[PARAM_MOD_P1]);
		params[PARAM_MOD_P2]->setValue(patch->ModulationParams[PARAM_MOD_P2]);
		params[PARAM_MOD_P3]->setValue(patch->ModulationParams[PARAM_MOD_P3]);
		params[PARAM_MOD_P4]->setValue(patch->ModulationParams[PARAM_MOD_P4]);

		for (auto param : params) {
			param->addObserver(this);
		}
	}

	virtual void updateControl(EControlId controlId, unsigned char value) {
		switch (controlId) {
		case EControlId::ModulationState:
			setIsPoweredOn(value == 1);
			notifyObservers();
			break;

		case EControlId::ModulationType:
			setDeviceIndex(value);
			resetParams();
			notifyObservers();
			break;

		case EControlId::ModulationP2:
			params[PARAM_MOD_P2]->setValue(value);
			break;

		case EControlId::ModulationP3:
			params[PARAM_MOD_P3]->setValue(value);
			break;

		case EControlId::ModulationP1:
			params[PARAM_MOD_P1]->setValue(value);
			break;

		case EControlId::ModulationP4:
			params[PARAM_MOD_P4]->setValue(value);
			break;

		default:
			break;
		}
	}
};
