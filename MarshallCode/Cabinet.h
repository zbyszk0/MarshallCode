#pragma once
#include "Device.h"

class Cabinet : public Device {
	void updateName();

public:
	Cabinet();

	~Cabinet() {
		for (auto param : params) {
			delete param;
		}
	}

	void serialize(RawPatch &patch) const {
		patch.CabinetIndex = getDeviceIndex();
		patch.CabinetPower = (getIsPoweredOn() ? 1 : 0);
	}

	void deserialize(RawPatch *patch) {
		setDeviceIndex(patch->CabinetIndex);
		setIsPoweredOn(patch->CabinetPower != 0);

		updateName();
	}

	virtual void updateControl(EControlId controlId, unsigned char value) {
		switch(controlId) {
		case EControlId::CabinetState:
			setIsPoweredOn(value == 1);
			notifyObservers();
			break;

		case EControlId::CabinetType:
			setDeviceIndex(value);
			notifyObservers();
			break;

		default:
			break;
		}
	}
};