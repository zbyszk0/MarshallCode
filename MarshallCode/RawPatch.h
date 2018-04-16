#pragma once

/*
[SysExMessage]
Start Byte: 240 MarshalId 1 = 0.33.21 Family.Model.Device = 48.16.3 Command: 115 Subcommand: 3

[Patch]
Patch Number: 1
Patch Name: Yngwie
Preamp: JTM45 Gain: 6.4 Bass: 1.1 Middle: 5.2 Treble: 6.3 Volume: 5.5 Gate: 2.8
Pedal: Distortion Speed: 0.2 Depth: 7.0 Mode: 4.1 Tone: 10.0
Delay: Vintage Time (ms): 512 Feedback: 4.1 Age: 2.6 Level: 5.1
Reverb: Hall Decay: 4.5 Pre-delay: 5.1 Tone: 7.0 Level: 5.1
Poweramp: Classic Marshall 100w Presence: 4.7 Resonance: 4.3
Cabinet: 1960

*/

struct RawPatch {
	unsigned char PatchNumber;
	unsigned char PatchName[19];

	// preamp
	unsigned char Gain;
	unsigned char Bass;
	unsigned char Middle;
	unsigned char Treble;
	unsigned char Volume;

	// pedal
	unsigned char PedalPower;
	unsigned char PedalIndex;
	unsigned char PedalParams[4];

	// preamp
	unsigned char PreampPower;
	unsigned char PreampIndex;
	unsigned char PreampGate;

	// Modulation
	unsigned char ModulationPower;
	unsigned char ModulationIndex;
	unsigned char ModulationParams[4];

	// Delay
	unsigned char DelayPower;
	unsigned char DelayIndex;
	unsigned char DelayParams[5];

	// Reverb
	unsigned char ReverbPower;
	unsigned char ReverbIndex;
	unsigned char ReverbParams[4];

	//PowerAmp
	unsigned char PowerAmpPower;
	unsigned char PowerAmpIndex;

	// Cabinet
	unsigned char CabinetPower;
	unsigned char CabinetIndex;

	// PowerAmpSettings
	unsigned char PowerAmpPresence;
	unsigned char PowerAmpResonance;

	unsigned char Padding[2];
	unsigned char DrId[4];
	unsigned char SysExEndByteOK;

	std::string toString() {
		std::stringstream out; 

		out << "[RawPatch]" << std::endl;

		out << "Patch Number: " << (int)this->PatchNumber << " Patch Name: " << this->PatchName << std::endl;


		if (this->PreampPower) {
			out << "Preamp: " << this->PreampIndex << " ";
		}

		out << "Gain: " << (int)this->Gain << " ";
		out << "Bass: " << (int)this->Bass << " ";
		out << "Middle: " << (int)this->Middle << " ";
		out << "Treble: " << (int)this->Treble << " ";
		out << "Volume: " << (int)this->Volume << " ";
		out << "PreampGate: " << (int)this->PreampGate << " ";
		out << std::endl;

		if (this->PedalPower) {
			out << "Pedal: " << (int)this->PedalIndex << " ";
			out << "p0: " << (int)this->PedalParams[0] << " ";
			out << "p1: " << (int)this->PedalParams[1] << " ";
			out << "p2: " << (int)this->PedalParams[2] << " ";
			out << "p3: " << (int)this->PedalParams[3] << " ";
			out << std::endl;
		}

		if (this->ModulationPower) {
			out << "Modulation: " << (int)this->ModulationIndex << " ";
			out << "p0: " << (int)this->ModulationParams[0] << " ";
			out << "p1: " << (int)this->ModulationParams[1] << " ";
			out << "p2: " << (int)this->ModulationParams[2] << " ";
			out << "p3: " << (int)this->ModulationParams[3] << " ";
			out << std::endl;
		}

		if (this->DelayPower) {
			out << "Modulation: " << (int)this->DelayIndex << " ";
			out << "p0: " << (int)(this->DelayParams[0] << 7 | this->DelayParams[1]) << " ";
			out << "p1: " << (int)this->DelayParams[2] << " ";
			out << "p2: " << (int)this->DelayParams[3] << " ";
			out << "p3: " << (int)this->DelayParams[4] << " ";
			out << std::endl;
		}

		if (this->ReverbPower) {
			out << "Reverb: " << (int)this->ReverbIndex << " ";
			out << "p0: " << (int)this->ReverbParams[0] << " ";
			out << "p1: " << (int)this->ReverbParams[1] << " ";
			out << "p2: " << (int)this->ReverbParams[2] << " ";
			out << "p3: " << (int)this->ReverbParams[3] << " ";
			out << std::endl;
		}

		if (this->PowerAmpPower) {
			out << "Poweramp: " << (int)this->PowerAmpIndex << " ";
			out << "Presence: " << (int)this->PowerAmpPresence << " ";
			out << "Resonance: " << (int)this->PowerAmpResonance << " ";
			out << std::endl;
		}

		if (this->CabinetPower) {
			out << "Cabinet: " << (int)this->CabinetIndex << std::endl;
		}

		out << std::endl;

		return out.str();
	}
};