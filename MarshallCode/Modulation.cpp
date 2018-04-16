#include "stdafx.h"
#include "Modulation.h"

static const char *ModulationNames[] = {
	"Chorus",
	"Flanger",
	"Phaser",
	"Tremolo"
};

bool Modulation::isInitialized = false;

static Parameter *ChorusParameters[4];
static Parameter *FlangerParameters[4];
static Parameter *PhaserParameters[4];
static Parameter *TremoloParameters[4];

static Parameter **Parameters[4];

static void StaticInitializeParameters() {
	ChorusParameters[0] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Speed", ""
	);
	ChorusParameters[1] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Depth", "" 
	);
	ChorusParameters[2] = new Parameter(
		EParameterType::ParamType_Selector, 0.f, 3, 1, 0, 0, 3, "Mode", "CLS,VIB"
	);
	ChorusParameters[3] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Tone", ""
	);

	FlangerParameters[0] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Speed", ""
	);
	FlangerParameters[1] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Depth", ""
	);
	FlangerParameters[2] = new Parameter(
		EParameterType::ParamType_Selector, 0.f, 2, 1, 0, 0, 3, "Mode", "JET,MET"
	);
	FlangerParameters[3] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Tone", ""
	);

	PhaserParameters[0] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Speed", ""
	);
	PhaserParameters[1] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Depth", ""
	);
	PhaserParameters[2] = new Parameter(
		EParameterType::ParamType_Selector, 0.f, 2, 1, 0, 0, 3, "Mode", "CLS,VBE"
	);
	PhaserParameters[3] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Tone", ""
	);

	TremoloParameters[0] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Speed", ""
	);
	TremoloParameters[1] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Depth", ""
	);
	TremoloParameters[2] = new Parameter(
		EParameterType::ParamType_Selector, 0.f, 2, 1, 0, 0, 3, "Mode", "VLV,SQR"
	);
	TremoloParameters[3] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Tone", ""
	);

	Parameters[0] = ChorusParameters;
	Parameters[1] = FlangerParameters;
	Parameters[2] = PhaserParameters;
	Parameters[3] = TremoloParameters;
}

void Modulation::StaticShutdown() {
	isInitialized = false;

	delete ChorusParameters[0];
	delete ChorusParameters[1];
	delete ChorusParameters[2];
	delete ChorusParameters[3];

	delete FlangerParameters[0];
	delete FlangerParameters[1];
	delete FlangerParameters[2];
	delete FlangerParameters[3];

	delete PhaserParameters[0];
	delete PhaserParameters[1];
	delete PhaserParameters[2];
	delete PhaserParameters[3];

	delete TremoloParameters[0];
	delete TremoloParameters[1];
	delete TremoloParameters[2];
	delete TremoloParameters[3];
}

Modulation::Modulation() {
	if (!isInitialized) {
		StaticInitializeParameters();
		isInitialized = true;
	}

	setDeviceIndex(0);
	setDeviceName(ModulationNames[0]);
	setParamCount(4);

	resetParams();
}

void Modulation::resetParams() {
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

	setDeviceName(ModulationNames[getDeviceIndex()]);
}
