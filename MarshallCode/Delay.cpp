#include "stdafx.h"
#include "Delay.h"

static const char *DelayNames[] = {
	"Studio",
	"Vintage",
	"Multi",
	"Reverse"
};

bool Delay::isInitialized = false;

static Parameter *StudioParameters[4];
static Parameter *VintageParameters[4];
static Parameter *MultiParameters[4];
static Parameter *ReverseParameters[4];

static Parameter **Parameters[4];

static void StaticInitializeParameters() {
	StudioParameters[0] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 4000.f, 1.f, 900, 0, 4000, "Time (ms)", ""
	);
	StudioParameters[1] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Feedback", ""
	);
	StudioParameters[2] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Frequency", ""
	);
	StudioParameters[3] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Level", ""
	);

	VintageParameters[0] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 4000.f, 1.f, 900, 0, 4000, "Time (ms)", ""
	);
	VintageParameters[1] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Feedback", ""
	);
	VintageParameters[2] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Age", ""
	);
	VintageParameters[3] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Level", ""
	);

	MultiParameters[0] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 4000.f, 1.f, 900, 0, 4000, "Time (ms)", ""
	);
	MultiParameters[1] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Feedback", ""
	);
	MultiParameters[2] = new Parameter(
		EParameterType::ParamType_Selector, 0.f, 5, 1, 0, 0, 5, "Tap Pattern", "-1-,-2-,-3-,-4-"
	);
	MultiParameters[3] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Level", ""
	);

	ReverseParameters[0] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 4000.f, 1.f, 900, 0, 4000, "Time (ms)", ""
	);
	ReverseParameters[1] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Feedback", ""
	);
	ReverseParameters[2] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Frequency", ""
	);
	ReverseParameters[3] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Level", ""
	);

	Parameters[0] = StudioParameters;
	Parameters[1] = VintageParameters;
	Parameters[2] = MultiParameters;
	Parameters[3] = ReverseParameters;
}

void Delay::StaticShutdown() {
	isInitialized = false;

	delete StudioParameters[0];
	delete StudioParameters[1];
	delete StudioParameters[2];
	delete StudioParameters[3];

	delete VintageParameters[0];
	delete VintageParameters[1];
	delete VintageParameters[2];
	delete VintageParameters[3];

	delete MultiParameters[0];
	delete MultiParameters[1];
	delete MultiParameters[2];
	delete MultiParameters[3];

	delete ReverseParameters[0];
	delete ReverseParameters[1];
	delete ReverseParameters[2];
	delete ReverseParameters[3];
}

Delay::Delay() {
	if (!isInitialized) {
		StaticInitializeParameters();
		isInitialized = true;
	}

	setDeviceIndex(0);
	setDeviceName(DelayNames[0]);
	setParamCount(4);

	resetParams();
}

void Delay::resetParams() {
	for (auto param : params) {
		delete param;
	}

	params.clear();

	for (int i = 0; i < getParamCount(); ++i) {
		Parameter *newParam = new Parameter(
			Parameters[getDeviceIndex()][i]->parameterType,
			Parameters[getDeviceIndex()][i]->min,
			Parameters[getDeviceIndex()][i]->max,
			Parameters[getDeviceIndex()][i]->stepSize,
			Parameters[getDeviceIndex()][i]->value,
			Parameters[getDeviceIndex()][i]->rangeMin,
			Parameters[getDeviceIndex()][i]->rangeMax,
			Parameters[getDeviceIndex()][i]->label.c_str(),
			""
		);

		params.emplace_back(newParam);

		newParam->options.clear();
		for (auto option : Parameters[getDeviceIndex()][i]->options) {
			if (!option.empty()) {
				params[i]->options.push_back(option);
			}
		}
	}

	setDeviceName(DelayNames[getDeviceIndex()]);
}