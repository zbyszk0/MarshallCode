#include "stdafx.h"
#include "Preamp.h"

static const char *PreampNames[] = {
	"JTM45",
	"Clean DSL",
	"Clean American",
	"Clean JVM",
	"Acoustic",
	"Bluesbreaker",
	"Plexi",
	"Crunch American",
	"JCM800",
	"50's British",
	"OD JVM",
	"OD DSL",
	"OD American",
	"OD Silver Jubilee",
	"Natural"
};

static Parameter JtmParameters[] = {
	{ EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 0.f, 0, 100, "Gate", "" },
};

static Parameter CleanDslParameters[] = {
	{ EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 0.f, 0, 100, "Gate", "" },
};

static Parameter CleanAmericanParameters[] = {
	{ EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 0.f, 0, 100, "Gate", "" },
};

static Parameter CleanJvmParameters[] = {
	{ EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 0.f, 0, 100, "Gate", "" },
};

static Parameter AcousticParameters[] = {
	{ EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 0.f, 0, 100, "Gate", "" },
};

static Parameter BluesbreakerParameters[] = {
	{ EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 0.f, 0, 100, "Gate", "" },
};

static Parameter PlexiParameters[] = {
	{ EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 0.f, 0, 100, "Gate", "" },
};

static Parameter CrunchAmericanParameters[] = {
	{ EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 0.f, 0, 100, "Gate", "" },
};

static Parameter JcmParameters[] = {
	{ EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 0.f, 0, 100, "Gate", "" },
};

static Parameter BritishParameters[] = {
	{ EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 0.f, 0, 100, "Gate", "" },
};

static Parameter OdJvmParameters[] = {
	{ EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 0.f, 0, 100, "Gate", "" },
};

static Parameter OdDslParameters[] = {
	{ EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 0.f, 0, 100, "Gate", "" },
};

static Parameter OdAmericanParameters[] = {
	{ EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 0.f, 0, 100, "Gate", "" },
};

static Parameter OdSilverParameters[] = {
	{ EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 0.f, 0, 100, "Gate", "" },
};

static Parameter NaturalParameters[] = {
	{ EParameterType::ParamType_Dial, 0.f, 10.f, 0.1f, 0.f, 0, 100, "Gate", "" },
};

static Parameter *Parameters[] = {
	JtmParameters,
	CleanDslParameters,
	CleanAmericanParameters,
	CleanJvmParameters,
	AcousticParameters,
	BluesbreakerParameters,
	PlexiParameters,
	CrunchAmericanParameters,
	JcmParameters,
	BritishParameters,
	OdJvmParameters,
	OdDslParameters,
	OdAmericanParameters,
	OdSilverParameters,
	NaturalParameters
};

Preamp::Preamp() {
	setDeviceIndex(0);
	setDeviceName(PreampNames[0]);
	setParamCount(1);

	resetParams();
}

void Preamp::resetParams() {
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

	setDeviceName(PreampNames[getDeviceIndex()]);
}
