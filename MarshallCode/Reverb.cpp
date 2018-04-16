#include "stdafx.h"
#include "Reverb.h"

static const char *ReverbNames[] = {
	"Spring",
	"Hall",
	"Room",
	"Stadium"
};

bool Reverb::isInitialized = false;

static Parameter *SpringParameters[4];
static Parameter *HallParameters[4];
static Parameter *RoomParameters[4];
static Parameter *StadiumParameters[4];

static Parameter **Parameters[4];

static void StaticInitializeParameters() {
	SpringParameters[0] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Decay", ""
	);
	SpringParameters[1] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Pre-delay", ""
	);
	SpringParameters[2] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Tone", ""
	);
	SpringParameters[3] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Level", ""
	);

	HallParameters[0] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Decay", ""
	);
	HallParameters[1] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Pre-delay", ""
	);
	HallParameters[2] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Tone", ""
	);
	HallParameters[3] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Level", ""
	);

	RoomParameters[0] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Decay", ""
	);
	RoomParameters[1] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Pre-delay", ""
	);
	RoomParameters[2] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Tone", ""
	);
	RoomParameters[3] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Level", ""
	);

	StadiumParameters[0] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Decay", ""
	);
	StadiumParameters[1] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Pre-delay", ""
	);
	StadiumParameters[2] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Tone", ""
	);
	StadiumParameters[3] = new Parameter(
		EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 50, 0, 100, "Level", ""
	);

	Parameters[0] = SpringParameters;
	Parameters[1] = HallParameters;
	Parameters[2] = RoomParameters;
	Parameters[3] = StadiumParameters;
}

void Reverb::StaticShutdown() {
	isInitialized = false;

	delete SpringParameters[0];
	delete SpringParameters[1];
	delete SpringParameters[2];
	delete SpringParameters[3];

	delete HallParameters[0];
	delete HallParameters[1];
	delete HallParameters[2];
	delete HallParameters[3];

	delete RoomParameters[0];
	delete RoomParameters[1];
	delete RoomParameters[2];
	delete RoomParameters[3];

	delete StadiumParameters[0];
	delete StadiumParameters[1];
	delete StadiumParameters[2];
	delete StadiumParameters[3];
}



Reverb::Reverb() {
	if (!isInitialized) {
		StaticInitializeParameters();
		isInitialized = true;
	}

	setDeviceIndex(0);
	setDeviceName(ReverbNames[0]);
	setParamCount(4);

	resetParams();
}

void Reverb::resetParams() {
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
			params[i]->options.push_back(option);
		}
	}

	setDeviceName(ReverbNames[getDeviceIndex()]);
}