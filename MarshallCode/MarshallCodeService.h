#pragma once

#include "Patch.h"
#include "RfcommChannel.h"
#include "MarshallSysExMessage.h"
#include "RawPatch.h" 
#include "Log.h"
#include <iomanip>
#include <queue>

// any references held when this macro is called are completely destroyed
// any methods invoked on such references have undefined behavior.
// call this only when you are completely sure that you are ready to shutdown the code service
#define SHUTDOWN_CODE_SERVICE() \
	MarshallCodeService::Get()->Shutdown(); \
	Delay::StaticShutdown(); \
	Pedal::StaticShutdown(); \
	Reverb::StaticShutdown(); \
	Modulation::StaticShutdown(); \
	Log::Get()->Shutdown();

static __forceinline void dumpBytes(const char* message, int len) {
	std::stringstream msg;
	msg << "Received Message: ";
	msg << "len: " << len << " bytes: [";
	for (int i = 0; i < len; ++i) {
		msg << (unsigned int)(message[i] & 255) << " ";
	}
	msg << "]";
	msg << std::endl;
	Log::Get()->Write(msg.str());
}

class IMarshallCodeObserver {
public:
	virtual void onStatusConnected() {};
	virtual void onStatusPatchesLoaded() {};
	virtual void onStatusTunerActivated() {};
	virtual void onStatusTunerDeactivated() {};

	// the active patch has been switched
	virtual void onActivePatchSwitched(unsigned char newActivePatch) {};

	// a control value was changed
	virtual void onControlChanged(EControlId controlId, unsigned char value) {};

	// received a message from the tuner
	virtual void onTunerEvent(std::string note, unsigned char intonation) {};

	// a patch has been modified
	virtual void onPatchModified(unsigned char id) {};

	// active patch has been modified
	virtual void onActivePatchModified() {};

	virtual ~IMarshallCodeObserver() {};
};

template <class funcType, typename param1Type = int, typename param2Type = int>
class Notification {
	funcType &FN;
	param1Type param1;
	param2Type param2;

public:
	Notification(funcType func, param1Type param1 = 0, param2Type param2 = 0):
		param1(param1), param2(param2) {
		FN = std::bind(FN, this, param1, param2);
	}

	void execute() {
		std::invoke(FN, param1, param2);
	}
};

class MarshallCodeService : public IPatchObserver {
	friend class Patch;

	const wchar_t *MARSHALL_CODE_DEVICE_PREFIX = TEXT("CODE");
	const wchar_t *MARSHALL_CODE_SPP_UUID = TEXT("{00001101-0000-1000-8000-00805F9B34FB}");
	const int MARSHALL_CODE_MAX_PATCHES = 100;

	static std::mutex mutex;
	static MarshallCodeService *instance;
	static Patch *nullPatch;

	RfcommChannel *codeRfcommChannel;
	Patch *activePatch = nullPatch;
	bool patchIsDirty;
	bool tunerIsOpen;
	bool isInitializing;
	std::thread *initThread;
	unsigned int loadedCount = 0;

	std::vector<Patch *> ampPatches;
	std::list<IMarshallCodeObserver *> observers;
	
	// HANDLE MESSAGES INTERNAL

	void onActivePatchSwitched(unsigned char newActivePatch) {
		if (newActivePatch == COMMAND_INCREMENT_PATCH) {
			activePatch = ampPatches[activePatch->getNumber() + 1];
		}
		else if (newActivePatch == (COMMAND_DECREMENT_PATCH)) {
			activePatch = ampPatches[activePatch->getNumber() - 1];

		}
		else if (newActivePatch >= 0 && newActivePatch < 100) {
			activePatch = ampPatches[newActivePatch];
		}

		std::stringstream msg;
		msg << "Patch Switched: "  << (int)(newActivePatch & 255) << std::endl;
		Log::Get()->Write(msg.str());
	}

	void onControlChanged(EControlId controlId, unsigned char value) {
		activePatch->updateControl(controlId, value);

		std::stringstream msg;
		msg << "Control Modified: " << (int)controlId << " " << (int)value << std::endl;
		Log::Get()->Write(msg.str());

		for (auto observer : observers) {
			observer->onControlChanged(controlId, value);
		}
	}

	void onTunerEvent(unsigned char midiNote, unsigned char intonation) {

		std::stringstream msg;
		msg << "Tuner: " << getNoteName(midiNote).c_str() << " " << (int)intonation << std::endl;
		Log::Get()->Write(msg.str());

		for (auto observer : observers) {
			observer->onTunerEvent(getNoteName(midiNote), intonation);
		}
	}

	void onPatchModified(unsigned char reply, RawPatch *rawPatch) {
		if (reply == COMMAND_GET_REPLY) {
			if (isInitializing) {
				loadedCount++;
			}

			ampPatches[rawPatch->PatchNumber]->deserialize(rawPatch);
			this->patchIsDirty = false;

			for (auto observer : observers) {
				observer->onPatchModified(rawPatch->PatchNumber);
			}
		}
		else if (reply == COMMAND_SET_REPLY_OK) {

			std::stringstream msg;
			msg << "Set Patch OK" << std::endl;
			Log::Get()->Write(msg.str());

			if (rawPatch->PatchNumber == activePatch->getNumber()) {
				this->patchIsDirty = false;
			}

			for (auto observer : observers) {
				observer->onPatchModified(rawPatch->PatchNumber);
			}
		}
		else if (reply == COMMAND_SET_REPLY_ERROR) {

			std::stringstream msg;
			msg << "Set Patch ERROR" << std::endl;
			Log::Get()->Write(msg.str());
		}
		else {

			std::stringstream msg;
			msg << "onPatchModified Unknown Reply " << (int)reply << std::endl;
			Log::Get()->Write(msg.str());
		}
	}

	void onActivePatchModified(unsigned char reply, RawPatch *rawPatch) {
		// reference the patch that already exists
		if (reply == COMMAND_GET_REPLY) {
			activePatch = ampPatches[rawPatch->PatchNumber];
			this->patchIsDirty = false;
		}
		else {

			std::stringstream msg;
			msg << "onActivePatchModified Unknown Reply " << (int)reply << std::endl;
			Log::Get()->Write(msg.str());
		}

		for (auto observer : observers) {
			observer->onActivePatchModified();
		}
	}

	// MESSAGE HANDLING

	void rfcomMidiBytesReceived(const char* message, int len) {
		switch(message[0] & 255) {
		case MESSAGE_CONTROL_CHANGE:
			Log::Get()->Write("MESSAGE_CONTROL_CHANGE\n");
			onControlChanged((EControlId)(message[1] & 255), message[2] & 255);
			break;

		case MESSAGE_TUNER:
			// subcommand = midi note, subcommand + 1 = intonation
			// intonation: 0 = too low, 1 = just right, 2 = too high
			onTunerEvent(message[1] & 255, message[2] & 255);
			Log::Get()->Write("MESSAGE_TUNER\n");
			break;

		default:
			Log::Get()->Write("UNRECOGNIZED_MESSAGE\n");
			break;
		}

		dumpBytes(message, len);
	}

	void rfcommMessageReceived(const char* message, int len) {


		if (!MarshallSysExMessage::IsValid(message, len)) {
			rfcomMidiBytesReceived(message, len);
			return;
		}

		const MarshallSysExMessage sysExMessage = 
			MarshallSysExMessage::FromNative(message, len);

		switch (sysExMessage.Command) {
			// personally, I don't really care about these.
			// just want to be able to control my amp.
			// if anyone wants to implement - feel free.
			case MESSAGE_DEVICE_INFO:
				Log::Get()->Write("MESSAGE_DEVICE_INFO\n");
				dumpBytes(message, len);
				break;

			case MESSAGE_FIRMWARE_UPDATE:
				Log::Get()->Write("MESSAGE_FIRMWARE_UPDATE\n");
				dumpBytes(message, len);
				break;

			case MESSAGE_FIRMWARE_DATA:
				Log::Get()->Write("MESSAGE_FIRMWARE_DATA\n");
				dumpBytes(message, len);
				break;

			case MESSAGE_BLUETOOTH:
				Log::Get()->Write("MESSAGE_BLUETOOTH\n");
				dumpBytes(message, len);
				break;

			case MESSAGE_SWITCH_PATCH:
				Log::Get()->Write("MESSAGE_SWITCH_PATCH\n");
				onActivePatchSwitched(sysExMessage.Subcommand);
				break;

			case MESSAGE_PATCH:
				onPatchModified(sysExMessage.Subcommand, (RawPatch *)(message + sizeof(MarshallSysExMessage)));
				break;

			case MESSAGE_ACTIVE_PATCH:
				onActivePatchModified(sysExMessage.Subcommand, (RawPatch *)(message + sizeof(MarshallSysExMessage)));
				break;

			default:
				Log::Get()->Write("UNKNOWN_MESSAGE\n");
				break;
		}

		dumpBytes(message, len);
	}

	void sendMessage_3Bytes(unsigned char a, unsigned char b, unsigned char c) {
		unsigned char rawMessage[4];
		rawMessage[0] = a;
		rawMessage[1] = b;
		rawMessage[2] = c;

		while(!codeRfcommChannel->write(rawMessage, 3)){}
	}

	void sendMessage_2Bytes(unsigned char a, unsigned char b) {
		unsigned char rawMessage[2];
		rawMessage[0] = a;
		rawMessage[1] = b;

		while (!codeRfcommChannel->write(rawMessage, 2)){}
	}

	void setPatch_Internal(const RawPatch &rawPatch) {
		MarshallSysExMessage message = MarshallSysExMessage::Create(MESSAGE_PATCH, COMMAND_SET);
		unsigned int messageLen = sizeof(MarshallSysExMessage) + sizeof(RawPatch);
		unsigned char *rawMessage = new unsigned char[messageLen];

		memcpy(rawMessage, &message, sizeof(MarshallSysExMessage));
		rawMessage[sizeof(MarshallSysExMessage)] = 0;
		memcpy(rawMessage + sizeof(MarshallSysExMessage), &rawPatch, sizeof(RawPatch));

		rawMessage[messageLen - 7] = 3;
		rawMessage[messageLen - 6] = 4;
		rawMessage[messageLen - 5] = 1;
		rawMessage[messageLen - 4] = 2;
		rawMessage[messageLen - 3] = 3;
		rawMessage[messageLen - 2] = 4;
		rawMessage[messageLen - 1] = MSG_END;

		while (!codeRfcommChannel->write(rawMessage, messageLen)){}
		delete[] rawMessage;
	}

	void getActivePatch_Internal() {
		MarshallSysExMessage message = MarshallSysExMessage::Create(MESSAGE_ACTIVE_PATCH, COMMAND_GET);
		unsigned int messageLen = sizeof(MarshallSysExMessage) + 2;
		unsigned char *rawMessage = new unsigned char[messageLen];

		memcpy(rawMessage, &message, sizeof(MarshallSysExMessage));
		rawMessage[messageLen - 2] = 0;
		rawMessage[messageLen - 1] = MSG_END;

		while (!codeRfcommChannel->write(rawMessage, messageLen)) {}
		delete[] rawMessage;
	}

	void getPatchById_Internal(unsigned char id) {
		MarshallSysExMessage message = MarshallSysExMessage::Create(MESSAGE_PATCH, COMMAND_GET);
		unsigned int messageLen = sizeof(MarshallSysExMessage) + 2;
		unsigned char *rawMessage = new unsigned char[messageLen];

		memcpy(rawMessage, &message, sizeof(MarshallSysExMessage));
		rawMessage[messageLen - 2] = id;
		rawMessage[messageLen - 1] = MSG_END;

		while (!codeRfcommChannel->write(rawMessage, messageLen)){}
		delete[] rawMessage;
	}

	// EVENTS

	void patchWantsToBeSaved(Patch *patch) {
		RawPatch patchRaw = patch->serialize();

		setPatch_Internal(patchRaw);

		std::stringstream msg;
		msg << "Patch Committed: " << patch->getNumber() << " " << patch->getName() << std::endl;
		Log::Get()->Write(msg.str());
	}

	void patchHasBeenModified(Patch *patch) {
		// patch is dirty;
		this->patchIsDirty = true; 
		
		std::stringstream msg;
		msg << "Patch Modified: " << patch->getNumber() << " " << patch->getName() << std::endl;
		Log::Get()->Write(msg.str());
	}

	// INIT SEQUENCE

	void loadNullPatches() {
		for (int i = 0; i < ampPatches.size(); ++i) {
			if (*ampPatches[i] == *nullPatch) {
				getPatchById_Internal(i);
				Sleep(100);
			}
		}
	}

	bool anyNullPatches() {
		for (int i = 0; i < ampPatches.size(); ++i) {
			if (*(ampPatches[i]) == *nullPatch) {
				return true;
			}
		}

		return false;
	}

	void loadPatches() {
		while (anyNullPatches()) {
			loadNullPatches();

			// yield for 1 second then check again
			Sleep(1000);
		}

		getActivePatch_Internal();
		Sleep(100);
	}

	void init() {
		while (!codeRfcommChannel->connect(
			MARSHALL_CODE_SPP_UUID, MARSHALL_CODE_DEVICE_PREFIX)) {

			Log::Get()->Write("Connection failed... Retrying in 1 second\n");
			Sleep(1000);
		}

		// populate initial patches
		loadPatches();
	}

	void runInit() {
		this->tunerIsOpen = this->patchIsDirty = false;

		if (!this->nullPatch) {
			this->nullPatch = new Patch();
		}

		for (int i = 0; i < MARSHALL_CODE_MAX_PATCHES; ++i) {
			this->ampPatches.push_back(new Patch());
		}

		for (auto patch : this->ampPatches) {
			patch->addObserver(this);
		}

		codeRfcommChannel = new RfcommChannel(
			std::bind(&MarshallCodeService::rfcommMessageReceived, this,
				std::placeholders::_1, std::placeholders::_2));

		for (auto observer : observers) {
			observer->onStatusConnected();
		}

		init();

		this->isInitializing = false;

		for (auto observer : observers) {
			observer->onStatusPatchesLoaded();
		}
	}

	MarshallCodeService() {
		this->isInitializing = true;
		initThread = new std::thread(std::bind(&MarshallCodeService::runInit, this));
		initThread->detach();
	}

	// NOTE: functionality unknown

	void changeMode(unsigned char unknown) {
		sendMessage_2Bytes(MESSAGE_CHANGE_PATCH_MODE, unknown);
	}

public:
	// PUBLIC INTERFACE

	virtual ~MarshallCodeService() {
		if (this->tunerIsOpen) {
			turnOffTuner();
		}

		delete codeRfcommChannel;
		delete initThread;

		for (auto patch : ampPatches) {
			delete patch;
		}

		delete nullPatch;
	}

	// release everything and destroy instance reference
	static void Shutdown() {
		if (instance) {
			delete instance;
			instance = nullptr;
		}
	}

	static MarshallCodeService *Get() {
		std::lock_guard<std::mutex> lock(mutex);

		if (instance == nullptr) {
			instance = new MarshallCodeService();
		}

		return instance;
	}

	void addObserver(IMarshallCodeObserver *observer) {
		observers.push_back(observer);
	}

	void removeObserver(IMarshallCodeObserver *observer) {
		observers.remove(observer);
	}

	void switchPatch(unsigned char switchToIndex) {
		sendMessage_2Bytes(MESSAGE_SWITCH_PATCH, switchToIndex);
		Sleep(100);

		for (auto observer : observers) {
			observer->onActivePatchSwitched(activePatch->getNumber());
		}
	}

	void incrementPatch() {
		sendMessage_2Bytes(MESSAGE_SWITCH_PATCH, COMMAND_INCREMENT_PATCH);
		Sleep(100);

		for (auto observer : observers) {
			observer->onActivePatchSwitched(activePatch->getNumber());
		}
	}

	void decrementPatch() {
		sendMessage_2Bytes(MESSAGE_SWITCH_PATCH, COMMAND_DECREMENT_PATCH);
		Sleep(100);

		for (auto observer : observers) {
			observer->onActivePatchSwitched(activePatch->getNumber());
		}
	}

	void changeControl(EControlId controlId, unsigned char value) {
		onControlChanged(controlId, value);

		std::stringstream msg;
		msg << "Modifying Control: " << (int)controlId << " " << (int)value << std::endl;
		Log::Get()->Write(msg.str());

		sendMessage_3Bytes(MESSAGE_CONTROL_CHANGE, (uint8_t)controlId, value);
		Sleep(100);
	}

	void turnOnTuner() {
		this->tunerIsOpen = true;
		changeControl(EControlId::Tuner, 1);
		
		Sleep(100);

		for (auto observer : observers) {
			observer->onStatusTunerActivated();
		}
	}

	void turnOffTuner() {
		this->tunerIsOpen = false;
		changeControl(EControlId::Tuner, 0);

		Sleep(100);

		for (auto observer : observers) {
			observer->onStatusTunerDeactivated();
		}
	}

	bool getTunerIsOpen() const {
		return this->tunerIsOpen;
	}

	bool getPatchIsDirty() const {
		return this->patchIsDirty;
	}

	Patch &getActivePatch() const {
		return *activePatch;
	}

	Patch &getPatchById(unsigned char id) const {
		return *ampPatches[id];
	}

	unsigned int getLoadedCount() const {
		return loadedCount;
	}

	bool getIsInitialized() const {
		return !isInitializing;
	}
};
