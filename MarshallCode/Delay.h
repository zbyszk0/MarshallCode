#pragma once
#include "Device.h"

class Delay : public Device {
	void resetParams();

	static bool isInitialized;
public:
	static const unsigned int PARAM_DELAY_P1 = 0;
	static const unsigned int PARAM_DELAY_P2 = 1;
	static const unsigned int PARAM_DELAY_P3 = 2;
	static const unsigned int PARAM_DELAY_P4 = 3;

	Delay();

	~Delay() {
		for (auto param : params) {
			delete param;
		}
	}

	static void StaticShutdown();

	void serialize(RawPatch &patch) const {
		patch.DelayIndex = getDeviceIndex();
		patch.DelayPower = (getIsPoweredOn() ? 1 : 0);

		int param0 = params[PARAM_DELAY_P1]->getValue();
		patch.DelayParams[0] = (param0 & (127 << 7)) >> 7;
		patch.DelayParams[1] = param0 & 127;
		patch.DelayParams[2] = params[PARAM_DELAY_P2]->getValue();
		patch.DelayParams[3] = params[PARAM_DELAY_P3]->getValue();
		patch.DelayParams[4] = params[PARAM_DELAY_P4]->getValue();
	}

	void deserialize(RawPatch *patch) {
		setDeviceIndex(patch->DelayIndex);
		setIsPoweredOn(patch->DelayPower != 0);

		resetParams();
		params[PARAM_DELAY_P1]->setValue(
			((patch->DelayParams[0] & 127) << 7) | 
			(patch->DelayParams[1] & 127));
		params[PARAM_DELAY_P2]->setValue(patch->DelayParams[2]);
		params[PARAM_DELAY_P3]->setValue(patch->DelayParams[3]);
		params[PARAM_DELAY_P4]->setValue(patch->DelayParams[4]);

		for (auto param : params) {
			param->addObserver(this);
		}
	}

	virtual void updateControl(EControlId controlId, unsigned char value) {
		switch (controlId) {
		case EControlId::DelayState:
			setIsPoweredOn(value == 1);
			notifyObservers();
			break;

		case EControlId::DelayType:
			setDeviceIndex(value);
			resetParams();
			notifyObservers();
			break;

		case EControlId::DelayTimeMSB:
			params[PARAM_DELAY_P1]->setValue((unsigned int)value << 7 | (((unsigned char)params[0]->getValue()) & 127));
			break;

		case EControlId::DelayTimeLSB:
			params[PARAM_DELAY_P1]->setValue(((unsigned int)params[0]->getValue() & (127 << 7)) | (value & 127));
			break;

		case EControlId::DelayP2:
			params[PARAM_DELAY_P2]->setValue(value);
			break;

		case EControlId::DelayP3:
			params[PARAM_DELAY_P3]->setValue(value);
			break;

		case EControlId::DelayP4:
			params[PARAM_DELAY_P4]->setValue(value);
			break;

		default:
			break;
		}
	}
};
