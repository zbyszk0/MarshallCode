#pragma once
#include "Cabinet.h"
#include "Poweramp.h"
#include "Reverb.h"
#include "Pedal.h"
#include "Preamp.h"
#include "Modulation.h"
#include "Delay.h"
#include "RawPatch.h"
#include "MarshallSysExMessage.h"

class Patch;

class IPatchObserver {
public:
	virtual void patchHasBeenModified(Patch *patch) = 0;
	virtual void patchWantsToBeSaved(Patch *patch) = 0;
};

class Patch : IDeviceObserver {
	unsigned char number;
	std::string name;

	unsigned char gain;
	unsigned char bass;
	unsigned char middle;
	unsigned char treble;
	unsigned char volume;

	Pedal *pedal;
	Preamp *preamp;
	Modulation *modulation;
	Delay *delay;
	Reverb *reverb;
	Poweramp *poweramp;
	Cabinet *cabinet;

	std::list<IPatchObserver *> observers;

public:
	Patch() {
		number = 0x00;
		gain = 0x00;
		bass = 0x00;
		middle = 0x00;
		treble = 0x00;
		volume = 0x00;

		pedal = new Pedal();
		pedal->addObserver(this);

		preamp = new Preamp();
		preamp->addObserver(this);

		modulation = new Modulation();
		modulation->addObserver(this);

		delay = new Delay();
		delay->addObserver(this);

		reverb = new Reverb();
		reverb->addObserver(this);

		poweramp = new Poweramp();
		poweramp->addObserver(this);

		cabinet = new Cabinet();
		cabinet->addObserver(this);
	}

	virtual ~Patch() {
		unsigned char number = 0;
		unsigned char gain = 0;
		unsigned char bass = 0;
		unsigned char middle = 0;
		unsigned char treble = 0;
		unsigned char volume = 0;

		delete pedal;
		delete preamp;
		delete modulation;
		delete delay;
		delete reverb;
		delete poweramp;
		delete cabinet;
	}

	Patch(const Patch &p) {
		this->name = p.name;
		this->number = p.number;
		this->gain = p.gain;
		this->bass = p.bass;
		this->middle = p.middle;
		this->treble = p.treble;
		this->volume = p.volume;

		this->pedal = new Pedal(*p.pedal);
		this->pedal->addObserver(this);

		this->preamp = new Preamp(*p.preamp);
		this->preamp->addObserver(this);

		this->modulation = new Modulation(*p.modulation);
		this->modulation->addObserver(this);

		this->delay = new Delay(*p.delay);
		this->delay->addObserver(this);

		this->reverb = new Reverb(*p.reverb);
		this->reverb->addObserver(this);

		this->poweramp = new Poweramp(*p.poweramp);
		this->poweramp->addObserver(this);

		this->cabinet = new Cabinet(*p.cabinet);
		this->cabinet->addObserver(this);
	}

	void deserialize(RawPatch *raw) {
		this->number = raw->PatchNumber;
		this->name = std::string((char *)raw->PatchName);
		this->gain = raw->Gain;
		this->bass = raw->Bass;
		this->middle = raw->Middle;
		this->treble = raw->Treble;
		this->volume = raw->Volume;

		this->pedal->deserialize(raw);
		this->preamp->deserialize(raw);
		this->modulation->deserialize(raw);
		this->delay->deserialize(raw);
		this->reverb->deserialize(raw);
		this->poweramp->deserialize(raw);
		this->cabinet->deserialize(raw);
	}

	RawPatch serialize() const {
		RawPatch patch;
		memset(&patch, 0x00, sizeof(patch));

		patch.PatchNumber = this->number;
		int nameLen = min(sizeof(patch.PatchName), this->name.length());
		memcpy(patch.PatchName, this->name.c_str(), nameLen);

		if (nameLen < 18) {
			memset(patch.PatchName + nameLen, 0x20, 18 - nameLen);
		}

		patch.Gain = this->gain;
		patch.Bass = this->bass;
		patch.Middle = this->middle;
		patch.Treble = this->treble;
		patch.Volume = this->volume;

		this->pedal->serialize(patch);
		this->preamp->serialize(patch);
		this->modulation->serialize(patch);
		this->delay->serialize(patch);
		this->reverb->serialize(patch);
		this->poweramp->serialize(patch);
		this->cabinet->serialize(patch);

		return patch;
	}

	std::string toString() {
		std::stringstream out;
		out << "[Patch]" << std::endl;

		out << "Name: " << this->name << " Number: " << (int)this->number << std::endl;

		out << "Gain: " << (int)this->gain << " ";
		out << "Bass: " << (int)this->bass << " ";
		out << "Middle: " << (int)this->middle << " ";
		out << "Treble: " << (int)this->treble << " ";
		out << "Volume: " << (int)this->volume << std::endl;

		if (pedal->getIsPoweredOn()) {
			out << pedal->toString() << std::endl;
		}
		if (preamp->getIsPoweredOn()) {
			out << preamp->toString() << std::endl;
		}
		if (modulation->getIsPoweredOn()) {
			out << modulation->toString() << std::endl;
		}
		if (delay->getIsPoweredOn()) {
			out << delay->toString() << std::endl;
		}
		if (reverb->getIsPoweredOn()) {
			out << reverb->toString() << std::endl;
		}
		if (poweramp->getIsPoweredOn()) {
			out << poweramp->toString() << std::endl;
		}
		if (cabinet->getIsPoweredOn()) {
			out << cabinet->toString() << std::endl;
		}
		return out.str();
	}

	void addObserver(IPatchObserver *patchObserver) {
		observers.emplace_back(patchObserver);
	}

	void notifyIsDirty() {
		for (auto observer: observers) {
			try {
				observer->patchHasBeenModified(this);
			}
			catch (std::exception &ex) {
				ex.what();
			}
		}
	}

	void notifyWantsSave() {
		for (auto observer : observers) {
			try {
				observer->patchWantsToBeSaved(this);
			}
			catch (std::exception &ex) {
				continue;
			}
		}
	}

	void updateControl(EControlId controlId, unsigned char value) {
		switch (controlId) {
			case EControlId::Gain:
				this->gain = value;
				notifyIsDirty();
				break;

			case EControlId::Bass:
				this->bass = value;
				notifyIsDirty();
				break;

			case EControlId::Middle:
				this->middle = value;
				notifyIsDirty();
				break;

			case EControlId::Treble:
				this->treble = value;
				notifyIsDirty();
				break;

			case EControlId::Volume:
				this->volume = value;
				notifyIsDirty();
				break;

			case EControlId::PedalState:
			case EControlId::PedalType:
			case EControlId::PedalP1:
			case EControlId::PedalP2:
			case EControlId::PedalP3:
			case EControlId::PedalP4:
				this->pedal->updateControl(controlId, value);
				break;

			case EControlId::AmpState:
			case EControlId::AmpType:
			case EControlId::AmpGate:
				this->preamp->updateControl(controlId, value);
				break;

			case EControlId::ModulationState:
			case EControlId::ModulationType:
			case EControlId::ModulationP2:
			case EControlId::ModulationP3:
			case EControlId::ModulationP1:
			case EControlId::ModulationP4:
				this->modulation->updateControl(controlId, value);
				break;

			case EControlId::DelayTimeMSB:
			case EControlId::DelayTimeLSB:
			case EControlId::DelayState:
			case EControlId::DelayType:
			case EControlId::DelayP2:
			case EControlId::DelayP3:
			case EControlId::DelayP4:
				this->delay->updateControl(controlId, value);
				break;

			case EControlId::ReverbState:
			case EControlId::ReverbType:
			case EControlId::ReverbP1:
			case EControlId::ReverbP2:
			case EControlId::ReverbP3:
			case EControlId::ReverbP4:
				this->reverb->updateControl(controlId, value);
				break;

			case EControlId::CabinetState:
			case EControlId::CabinetType:
				this->cabinet->updateControl(controlId, value);
				break;

			case EControlId::PowerAmpState:
			case EControlId::PowerAmpType:
			case EControlId::PowerAmpPresence:
			case EControlId::PowerAmpResonance:
				this->poweramp->updateControl(controlId, value);
				break;

			default:
				break;
		}
	}

	void deviceHasBeenModified(Device *device) {
		notifyIsDirty();
	}

	void commit() {
		notifyWantsSave();
	}

	int getNumber() {
		return number;
	}

	std::string &getName() {
		return name;
	}

	Pedal &getPedal() {
		return *pedal;
	}

	Preamp &getPreamp() {
		return *preamp;
	}

	Modulation &getModulation() {
		return *modulation;
	}

	Delay &getDelay() {
		return *delay;
	}

	Reverb &getReverb() {
		return *reverb;
	}

	Poweramp &getPoweramp() {
		return *poweramp;
	}

	Cabinet &getCabinet() {
		return *cabinet;
	}

	unsigned char getGain() {
		return gain;
	}

	unsigned char getBass() {
		return bass;
	}

	unsigned char getMiddle() {
		return middle;
	}

	unsigned char getTreble() {
		return treble;
	}

	unsigned char getVolume() {
		return volume;
	}

	void setName(const char* name) {
		this->name = name;
	}

	bool operator==(const Patch &other) const {
		if (number != other.number) {
			return false;
		}

		if (name != other.name) {
			return false;
		}

		if (gain != other.gain) {
			return false;
		}

		if (bass != other.bass) {
			return false;
		}

		if (middle != other.middle) {
			return false;
		}

		if (treble != other.treble) {
			return false;
		}

		if (volume != other.volume) {
			return false;
		}

		if (!(pedal && other.pedal && *pedal == *other.pedal)) {
			return false;
		}

		if (!(preamp && other.preamp && *preamp == *other.preamp)) {
			return false;
		}

		if (!(delay && other.delay && *delay == *other.delay)) {
			return false;
		}

		if (!(reverb && other.reverb && *reverb == *other.reverb)) {
			return false;
		}

		if (!(cabinet && other.cabinet && *cabinet == *other.cabinet)) {
			return false;
		}

		if (!(modulation && other.modulation && *modulation == *other.modulation)) {
			return false;
		}

		if (!(poweramp && other.poweramp && *poweramp == *other.poweramp)) {
			return false;
		}

		return true;
	}

	Device *getDeviceByControlId(EControlId controlId) {
		switch (controlId) {

			case EControlId::DelayTimeMSB:
			case EControlId::DelayTimeLSB:
			case EControlId::DelayState:
			case EControlId::DelayType:
			case EControlId::DelayP2:
			case EControlId::DelayP3:
			case EControlId::DelayP4:
				return delay;
			case EControlId::AmpType:
			case EControlId::AmpState:
				return preamp;
			case EControlId::PedalState:
			case EControlId::PedalType:
			case EControlId::PedalP1:
			case EControlId::PedalP2:
			case EControlId::PedalP3:
			case EControlId::PedalP4:
				return pedal;
			case EControlId::ModulationState:
			case EControlId::ModulationType:
			case EControlId::ModulationP2:
			case EControlId::ModulationP3:
			case EControlId::ModulationP1:
			case EControlId::ModulationP4:
				return modulation;
			case EControlId::ReverbState:
			case EControlId::ReverbType:
			case EControlId::ReverbP1:
			case EControlId::ReverbP2:
			case EControlId::ReverbP3:
			case EControlId::ReverbP4:
				return reverb;
			case EControlId::PowerAmpState:
			case EControlId::PowerAmpType:
			case EControlId::PowerAmpPresence:
			case EControlId::PowerAmpResonance:
				return poweramp;
			case EControlId::CabinetType:
			case EControlId::CabinetState:
				return cabinet;

			default:
				return nullptr;
		}
	}
	Parameter *getParameterByControlId(EControlId controlId) {
		switch (controlId) {
			case EControlId::AmpGate:
				return this->preamp->getParameter(Preamp::PARAM_GATE);
			case EControlId::PowerAmpPresence:
				return this->poweramp->getParameter(Poweramp::PARAM_PRESENCE);
			case EControlId::PowerAmpResonance:
				return this->poweramp->getParameter(Poweramp::PARAM_RESONANCE);
			case EControlId::PedalP1:
				return this->pedal->getParameter(Pedal::PARAM_PEDAL_P1);
			case EControlId::PedalP2:
				return this->pedal->getParameter(Pedal::PARAM_PEDAL_P2);
			case EControlId::PedalP3:
				return this->pedal->getParameter(Pedal::PARAM_PEDAL_P3);
			case EControlId::PedalP4:
				return this->pedal->getParameter(Pedal::PARAM_PEDAL_P4);
			case EControlId::ModulationP1:
				return this->modulation->getParameter(Modulation::PARAM_MOD_P1);
			case EControlId::ModulationP2:
				return this->modulation->getParameter(Modulation::PARAM_MOD_P2);
			case EControlId::ModulationP3:
				return this->modulation->getParameter(Modulation::PARAM_MOD_P3);
			case EControlId::ModulationP4:
				return this->modulation->getParameter(Modulation::PARAM_MOD_P4);
			case EControlId::DelayTimeMSB:
			case EControlId::DelayTimeLSB:
				return this->delay->getParameter(Delay::PARAM_DELAY_P1);
			case EControlId::DelayP2:
				return this->delay->getParameter(Delay::PARAM_DELAY_P2);
			case EControlId::DelayP3:
				return this->delay->getParameter(Delay::PARAM_DELAY_P3);
			case EControlId::DelayP4:
				return this->delay->getParameter(Delay::PARAM_DELAY_P4);
			case EControlId::ReverbP1:
				return this->reverb->getParameter(Reverb::PARAM_REVERB_P1);
			case EControlId::ReverbP2:
				return this->reverb->getParameter(Reverb::PARAM_REVERB_P2);
			case EControlId::ReverbP3:
				return this->reverb->getParameter(Reverb::PARAM_REVERB_P3);
			case EControlId::ReverbP4:
				return this->reverb->getParameter(Reverb::PARAM_REVERB_P4);

			default:
				return nullptr;
		}
	}
};
