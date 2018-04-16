#include "stdafx.h"
#include "Poweramp.h"

static const char *PowerampNames[] = {
	"Classic Marshall 100w",
	"Vintage Marshall 30w",
	"British Class A",
	"American Class A/B"
};

static Parameter ClassicParameters[] = {
	{ EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 0.f, 0, 100, "Presence", "" },
	{ EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 0.f, 0, 100, "Resonance", "" },
};

static Parameter VintageParameters[] = {
	{ EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 0.f, 0, 100, "Presence", "" },
	{ EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 0.f, 0, 100, "Resonance", "" },
};

static Parameter BritishParameters[] = {
	{ EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 0.f, 0, 100, "Presence", "" },
	{ EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 0.f, 0, 100, "Resonance", "" },
};

static Parameter AmericanParameters[] = {
	{ EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 0.f, 0, 100, "Presence", "" },
	{ EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 0.f, 0, 100, "Resonance", "" },
};

static Parameter *Parameters[] = {
	ClassicParameters,
	VintageParameters,
	BritishParameters,
	AmericanParameters
};

Poweramp::Poweramp() {
	setDeviceIndex(0);
	setDeviceName(PowerampNames[0]);
	setParamCount(2);

	resetParams();
}

void Poweramp::resetParams() {
	for (auto param : params) {
		delete param;
	}

	params.clear();

	for (int i = 0; i < getParamCount(); ++i) {
		Parameter *newParam = new Parameter(
			Parameters[getDeviceIndex()][i].parameterType,
			Parameters[getDeviceIndex()][i].min,
			Parameters[getDeviceIndex()][i].max,
			Parameters[getDeviceIndex()][i].stepSize,
			Parameters[getDeviceIndex()][i].value,
			Parameters[getDeviceIndex()][i].rangeMin,
			Parameters[getDeviceIndex()][i].rangeMax,
			Parameters[getDeviceIndex()][i].label.c_str(),
			""
		);

		params.emplace_back(newParam);

		newParam->options.clear();
		for (auto option : Parameters[getDeviceIndex()][i].options) {
			params[i]->options.push_back(option);
		}
	}

	setDeviceName(PowerampNames[getDeviceIndex()]);
}
