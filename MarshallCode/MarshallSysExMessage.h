#pragma once

const unsigned char MSG_START = 240;
const unsigned char MSG_END = 247;

const unsigned char MARSHALL_ID_0 = 0;
const unsigned char MARSHALL_ID_1 = 33;
const unsigned char MARSHALL_ID_2 = 21;

const unsigned char MARSHALL_FAMILY = 127;
const unsigned char MARSHALL_MODEL = 127;
const unsigned char MARSHALL_DEVICE = 127;

const unsigned char MESSAGE_TUNER = 160;
const unsigned char MESSAGE_DEVICE_INFO = 17;
const unsigned char MESSAGE_FIRMWARE_UPDATE = 33;
const unsigned char MESSAGE_FIRMWARE_DATA = 38;
const unsigned char MESSAGE_BLUETOOTH = 98;
const unsigned char MESSAGE_CONTROL_CHANGE = 176;
const unsigned char MESSAGE_SWITCH_PATCH = 192;
const unsigned char MESSAGE_PATCH = 114;
const unsigned char MESSAGE_ACTIVE_PATCH = 115;
const unsigned char MESSAGE_CHANGE_PATCH_MODE = 30;

const unsigned char COMMAND_INCREMENT_PATCH = 101;
const unsigned char COMMAND_DECREMENT_PATCH = 108;
const unsigned char COMMAND_GET = 1;
const unsigned char COMMAND_SET = 2;
const unsigned char COMMAND_GET_REPLY = 3;
const unsigned char COMMAND_SET_REPLY_OK = 4;
const unsigned char COMMAND_SET_REPLY_ERROR = 5;
const unsigned char COMMAND_FIRMWARE_READY_REPLY = 1;
const unsigned char COMMAND_9_NAMES_REPLY = 6;
const unsigned char COMMAND_NAME_REPLY = 7;
const unsigned char COMMAND_3_NAMES_REPLY = 8;

enum class EControlId : uint8_t {

	Tuner = 52,

	Gain = 70,
	Bass,
	Middle,
	Treble,
	Volume,

	PedalState,
	PedalType,
	PedalP1,
	PedalP2,
	PedalP3,
	PedalP4,

	AmpState,
	AmpType,
	AmpGate,

	ModulationState = 85,
	ModulationType,
	ModulationP2,
	ModulationP3 = 89,
	ModulationP1,
	ModulationP4 = 102,

	DelayTimeMSB = 31,
	DelayTimeLSB = 63,
	DelayState = 103,
	DelayType,
	DelayP2,
	DelayP3,
	DelayP4,

	ReverbState,
	ReverbType,
	ReverbP1,
	ReverbP2,
	ReverbP3,
	ReverbP4,

	PowerAmpState,
	PowerAmpType,

	CabinetState,
	CabinetType,

	PowerAmpPresence,
	PowerAmpResonance,
	
};

const std::string notenames[] = {
	"G#/Ab",
	"A",
	"A#/Bb",
	"B",
	"C",
	"C#/Db",
	"D",
	"D#/Eb",
	"E",
	"F",
	"F#/Gb",
	"G"
};

std::string __forceinline getNoteName(unsigned char note) {
	auto baseNote = notenames[note % 12];
	int octave = (note / 12) + 1;
	std::stringstream out; 
	out << baseNote << octave;
	return out.str();
}

struct MarshallSysExMessage {
	unsigned char SysExStartByte;

	unsigned char MarshallId[3];
	unsigned char Family;
	unsigned char Model;
	unsigned char Device;

	unsigned char Command;
	unsigned char Subcommand;

	// instance methods
	std::string toString() {
		std::stringstream out; 
		out << "[SysExMessage]" << std::endl;
		out << "Start Byte: " << (int)this->SysExStartByte << std::endl;
		out << "MarshalId 1 = " <<
			(int)this->MarshallId[0] << "." <<
			(int)this->MarshallId[1] << "." <<
			(int)this->MarshallId[2] << std::endl;
		out << "Family.Model.Device = " <<
			(int)this->Family << "." <<
			(int)this->Model << "." <<
			(int)this->Device << std::endl;
		out << "Command: " << (int)this->Command << " Subcommand: " << (int)this->Subcommand << std::endl;
		out << std::endl;
		return out.str();
	}

	static MarshallSysExMessage Create(unsigned char command, unsigned char subcommand) {
		MarshallSysExMessage message;

		message.SysExStartByte = MSG_START;

		message.MarshallId[0] = MARSHALL_ID_0;
		message.MarshallId[1] = MARSHALL_ID_1;
		message.MarshallId[2] = MARSHALL_ID_2;

		message.Family = MARSHALL_FAMILY;
		message.Device = MARSHALL_DEVICE;
		message.Model = MARSHALL_MODEL;

		message.Command = command;
		message.Subcommand = subcommand;

		return message;
	}

	// static methods
	static bool IsValid(const char *message, int len) {
		const MarshallSysExMessage *sysExMessage = (MarshallSysExMessage *)message;

		if (len < 9) {
			return false;
		}

		if (sysExMessage->SysExStartByte != MSG_START) {
			return false;
		}

		if (((unsigned char)message[len - 1]) != MSG_END) {
			return false;
		}

		if (!(sysExMessage->MarshallId[0] == MARSHALL_ID_0 &&
			sysExMessage->MarshallId[1] == MARSHALL_ID_1 &&
			sysExMessage->MarshallId[2] == MARSHALL_ID_2)) {

			return false;
		}

		return true;
	}

	static MarshallSysExMessage FromNative(const char *message, int len) {
		MarshallSysExMessage returnMessage;
		memcpy(&returnMessage, message, min(len, ((int)sizeof(MarshallSysExMessage))));
		return returnMessage;
	}

};