#pragma once

#include "Parameter.h"
#include "RawPatch.h"
#include "MarshallSysExMessage.h"

class Device;

class IDeviceObserver {
public:
	virtual void deviceHasBeenModified(Device *device) = 0;
};

class Device : public IParameterObserver {
	bool isPoweredOn;
	unsigned char deviceIndex;
	std::string deviceName;
	int paramCount;

	std::list<IDeviceObserver *> observers;

protected:
	std::vector<Parameter *> params;

	void setParamCount(int paramCount) { this->paramCount = paramCount;  }
	void setDeviceName(std::string deviceName) { this->deviceName = deviceName; }

public:
	Device(bool isPoweredOn = false, unsigned deviceIndex = 0) : isPoweredOn(isPoweredOn), deviceIndex(deviceIndex) {
		deviceName = "error";
	}

	virtual ~Device() {
	}
	Device(const Device &d) {
		this->isPoweredOn = d.isPoweredOn;
		this->deviceIndex = d.deviceIndex;
		this->deviceName = d.deviceName;

		for (auto param : d.params) {
			auto copy = new Parameter(*param);
			copy->addObserver(this);
			params.emplace_back(copy);
		}

		paramCount = params.size();
	}

	virtual void serialize(RawPatch &patch) const {}
	virtual void deserialize(RawPatch *patch) {}
	virtual void updateControl(EControlId controlId, unsigned char value) {}

	virtual void parameterHasBeenModified(Parameter *param) {
		notifyObservers();
	}

	void addObserver(IDeviceObserver *deviceObserver) {
		observers.emplace_back(deviceObserver);
	}

	int getParamCount() const { return paramCount;  };
	std::string getName() const { return deviceName;  };

	bool getIsPoweredOn() const { return isPoweredOn; }
	unsigned char getDeviceIndex() const { return deviceIndex; }

	// TODO: NOTIFY
	void setIsPoweredOn(bool isPoweredOn) { this->isPoweredOn = isPoweredOn; }
	void setDeviceIndex(unsigned char deviceIndex) { this->deviceIndex = deviceIndex; }

	Parameter *getParameter(unsigned char index) {
		unsigned char safeIndex = index % params.size();
		return params[safeIndex];
	}

	void notifyObservers() {
		for (auto observer : observers) {
			try {
				observer->deviceHasBeenModified(this);
			}
			catch (std::exception &ex) {
				ex.what();
			}
		}
	}

	std::string toString() {
		std::stringstream out;

		out << getName() << " ";
		for (auto param : params) {
			out << param->toString();
		}

		return out.str();
	}

	// todo: hash
	bool operator==(const Device &other) const {
		if (deviceName != other.deviceName) {
			return false;
		}

		if (deviceIndex != other.deviceIndex) {
			return false;
		}

		if (paramCount != other.paramCount) {
			return false;
		}

		if (params.size() != other.params.size()) {
			return false;
		}

		for (int i = 0; i < params.size(); ++i) {
			if (!(*params[i] == *other.params[i])) {
				return false;
			}
		}

		return true;
	}
};
