// MarshallCode.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "MarshallCode.h"
#include <iostream>
#include <synchapi.h>

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "Bthprops.lib")
#pragma comment (lib, "MarshallCode.lib")

// sample observer
class MyObserver : public IMarshallCodeObserver {
public:
	void onStatusConnected() {
		std::cout << "onStatusConnected" << std::endl;
	};

	void onStatusPatchesLoaded() {
		std::cout << "onStatusPatchesLoaded" << std::endl;
	};

	void onStatusTunerActivated() {
		std::cout << "onStatusTunerActivated" << std::endl;
	};

	void onStatusTunerDeactivated() {
		std::cout << "onStatusTunerDeactivated" << std::endl;
	};

	// the active patch has been switched
	void onActivePatchSwitched(unsigned char newActivePatch) {
		std::cout << "onActivePatchChanged: " << 
			(int)newActivePatch << std::endl;
	};

	// a control value was changed
	void onControlChanged(EControlId controlId, unsigned char value) {
		std::cout << "onControlChanged: " << 
			(int)controlId << " " << (int)value << std::endl;
	};

	// received a message from the tuner
	void onTunerEvent(unsigned char midiNote, unsigned char intonation) {
		std::cout << "onTunerEvent: " << 
			(int)midiNote << " " << (int)intonation << std::endl;
	};

	// a patch has been modified
	void onPatchModified(unsigned char id) {
		std::cout << "onPatchModified: " << 
			(int)id << " " << std::endl;
	};

	// active patch has been modified
	void onActivePatchModified() {
		std::cout << "onActivePatchModified" << std::endl;
	};
};

int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// example observer - receives notifications from the marshall code service
	MyObserver exampleObserver;

	// get marshall code service ref
	MarshallCodeService *codeService = MarshallCodeService::Get();

	// add an observer to be notified about various events
	codeService->addObserver(&exampleObserver);
	
	// wait until the service is initialized
	while (!codeService->getIsInitialized()) { Sleep(1); }

	// get the current active patch
	std::cout << codeService->getActivePatch().toString();

	// get all patches
	for (int i = 0; i < 100; ++i) {
		std::cout << codeService->getPatchById(i).toString();
	}

	// get an individual parameter
	auto param = codeService->getPatchById(11).getPreamp().getParameter(Preamp::PARAM_GATE);

	// get that parameters value
	std::cout << "Param Value: " << param->getValue() << std::endl;

	// change an individual control 
	codeService->changeControl(EControlId::Treble, 20);
	std::cout << codeService->getActivePatch().toString();

	// change random controls (TORTURE TEST)
	for (int i = 0; i < 1000; ++i) {
		unsigned char control = (rand() % 50) + 70;
		unsigned char value = (rand() % 100);

		if (control == (unsigned char)EControlId::ReverbType ||
			control == (unsigned char)EControlId::ModulationType ||
			control == (unsigned char)EControlId::DelayType ||
			control == (unsigned char)EControlId::PedalType ||
			control == (unsigned char)EControlId::PowerAmpType) {
			value = value % 4;
		}

		if (control == (unsigned char)EControlId::CabinetType) {
			value = value % 8;
		}

		if (control == (unsigned char)EControlId::AmpType) {
			value = value % 15;
		}

		codeService->changeControl((EControlId)control, value);
	}

	// commit the modified patch (save)
	codeService->getPatchById(11).commit();
	Sleep(75);

	// get an individual patch by ID
	std::cout << codeService->getPatchById(11).toString() << std::endl;

	// switch to an arbitrary patch
	codeService->switchPatch(56);
	std::cout << codeService->getActivePatch().toString();

	// switch to the next patch
	codeService->incrementPatch();
	std::cout << codeService->getActivePatch().toString();

	// switch to the previous patch
	codeService->decrementPatch();
	std::cout << codeService->getActivePatch().toString();

	codeService->decrementPatch();
	std::cout << codeService->getActivePatch().toString();

	// is the tuner open / on? 
	std::cout << "Tuner Is Open? " << codeService->getTunerIsOpen() << std::endl;

	// turn on the tuner
	codeService->turnOnTuner();
	std::cout << "Tuner Is Open? " << codeService->getTunerIsOpen() << std::endl;

	Sleep(10000);
	
	// turn off the tuner
	codeService->turnOffTuner();
	std::cout << "Tuner Is Open? " << codeService->getTunerIsOpen() << std::endl;

	// does the patch have uncommitted changes? 
	std::cout << "Patch Is Dirty? " << codeService->getPatchIsDirty() << std::endl;

	// change an individual control 
	codeService->changeControl(EControlId::Middle, 20);
	std::cout << codeService->getActivePatch().toString();

	codeService->getActivePatch().commit();
	Sleep(100);
	std::cout << "Patch Is Dirty? " << codeService->getPatchIsDirty() << std::endl;

	return 0;
}