#include "stdafx.h"
#include "Cabinet.h"

static const char *CabinetNames[] = {
	"1960",
	"1960V",
	"1960X",
	"1960HW",
	"1936",
	"1936V",
	"1912",
	"1974CX"
};

Cabinet::Cabinet() {
	setDeviceIndex(0);
	setDeviceName(CabinetNames[0]);
	setParamCount(0);
}

void Cabinet::updateName() {
	setDeviceName(CabinetNames[getDeviceIndex()]);
}
