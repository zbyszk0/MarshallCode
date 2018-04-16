#include "stdafx.h"
#include "Pedal.h"

static const char *PedalNames[] = {
	"Compressor",
	"Distortion",
	"Auto Wah",
	"Pitch Shifter"
};

bool Pedal::isInitialized = false;

static Parameter *CompressorParameters[4];
static Parameter *DistortionParameters[4];
static Parameter *AutoWahParameters[4];
static Parameter *PitchShifterParameters[4];

static Parameter **Parameters[4];

static void StaticInitializeParameters() {
	CompressorParameters[0] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Tone", ""
	);
	CompressorParameters[1] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Ratio", ""
	);
	CompressorParameters[2] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Compression", ""
	);
	CompressorParameters[3] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Level", ""
	);

	DistortionParameters[0] = new Parameter(
		EParameterType::ParamType_Selector, 0.f, 4, 1, 0, 0, 4, "Mode", "GUV,ODR,DIS"
	);
	DistortionParameters[1] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Drive", ""
	);
	DistortionParameters[2] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Tone", ""
	);
	DistortionParameters[3] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Level", ""
	);

	AutoWahParameters[0] = new Parameter(
		EParameterType::ParamType_Selector, 0.f, 3, 1, 0, 0, 3, "Mode", "ENV,LFO"
	);
	AutoWahParameters[1] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Frequency", ""
	);
	AutoWahParameters[2] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Sensitivity", ""
	);
	AutoWahParameters[3] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Res", ""
	);

	PitchShifterParameters[0] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Semitone", ""
	);
	PitchShifterParameters[1] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Fine", ""
	);
	PitchShifterParameters[2] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Regen", ""
	);
	PitchShifterParameters[3] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Mix", ""
	);

	Parameters[0] = CompressorParameters;
	Parameters[1] = DistortionParameters;
	Parameters[2] = AutoWahParameters;
	Parameters[3] = PitchShifterParameters;
}

void Pedal::StaticShutdown() {
	isInitialized = false;

	delete CompressorParameters[0];
	delete CompressorParameters[1];
	delete CompressorParameters[2];
	delete CompressorParameters[3];

	delete DistortionParameters[0];
	delete DistortionParameters[1];
	delete DistortionParameters[2];
	delete DistortionParameters[3];

	delete AutoWahParameters[0];
	delete AutoWahParameters[1];
	delete AutoWahParameters[2];
	delete AutoWahParameters[3];

	delete PitchShifterParameters[0];
	delete PitchShifterParameters[1];
	delete PitchShifterParameters[2];
	delete PitchShifterParameters[3];
}


Pedal::Pedal() {
	if (!isInitialized) {
		StaticInitializeParameters();
		isInitialized = true;
	}

	setDeviceIndex(0);
	setDeviceName(PedalNames[0]);
	setParamCount(4);

	resetParams();
}

void Pedal::resetParams() {
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

		newParam->options.clear();
		params.emplace_back(newParam);

		for (auto option : Parameters[getDeviceIndex()][i]->options) {
			if (!option.empty()) {
				params[i]->options.push_back(option);
			}
		}
	}

	setDeviceName(PedalNames[getDeviceIndex()]);
}