#pragma once
#include <iostream>

class Parameter;

enum class EParameterType : uint8_t {
	ParamType_Selector,
	ParamType_Dial
};


class IParameterObserver {
public:
	virtual void parameterHasBeenModified(Parameter *param) = 0;
};

class Parameter {
	EParameterType parameterType;

	// for selectors this is [0, maxParamNameIndex]
	float min;
	float max;
	float stepSize;
	float value;

	int rangeMin;
	int rangeMax;

	std::string label;
	std::vector<std::string> options;

	std::list<IParameterObserver *> observers;
public:
	Parameter(EParameterType parameterType, float min, float max,	float stepSize,
		float value, int rangeMin, int rangeMax, const char *label, const char *options) {

		this->parameterType = parameterType;
		this->min = min;
		this->max = max;
		this->stepSize = stepSize;
		this->value = value;
		this->rangeMin = rangeMin;
		this->rangeMax = rangeMax;
		this->label = label;

		if (this->parameterType == EParameterType::ParamType_Selector) {
			std::string temp = options;

			unsigned int optionCount = 0;
			this->rangeMin = this->min = 0;
			this->stepSize = 1;

			auto idx = temp.find_first_of(',');
			auto offset = 0;

			do {
				auto option = temp.substr(offset, idx - offset);
				this->options.push_back(option);
				offset = idx + 1;
			} while ((idx = temp.find(',', offset)) != std::string::npos);

			auto option = temp.substr(offset, idx - temp.length());
			this->options.push_back(option);

			this->rangeMax = this->max = this->options.size();
		}
	}

	void addObserver(IParameterObserver *paramObserver) {
		observers.emplace_back(paramObserver);
	}

	std::string toString() {
		std::stringstream out; 
		out << this->label << ": ";
		if (this->isDial()) {
			out << getFriendlyValue() << " ";
		}
		else if (this->isSelector()) {
			out << getSelection() << " ";
		}
		return out.str();
	}

	float getValue() {
		return this->value;
	}

	float getStepSize() {
		return this->stepSize;
	}

	void setValue(unsigned short value) {
		if (value != this->value) {
			this->value = value;

			notifyObservers();
		}
	}

	std::string getLabel() const {
		return label;
	}

	std::string getSelection() const {
		if (this->value >= 0 && this->value < options.size()) {
			return options[value];
		}

		return "ERROR";
	}

	float getFriendlyValue() const {
		return (max - min) / (rangeMax - rangeMin) * this->value;
	}

	bool isSelector() const {
		return this->parameterType == EParameterType::ParamType_Selector;
	}

	bool isDial() const {
		return this->parameterType == EParameterType::ParamType_Dial;
	}

	unsigned char getMin() const {
		return rangeMin;
	}

	unsigned int getMax() const {
		return rangeMax;
	}

	void notifyObservers() {
		for (auto observer : observers) {
			try {
				observer->parameterHasBeenModified(this);
			}
			catch (std::exception &ex) {
				ex.what();
			}
		}
	}

	std::vector<std::string> &getOptions() {
		return options;
	}

	// TODO: hash values
	bool operator==(const Parameter &other) const {
		if (((uint8_t)parameterType != (uint8_t)other.parameterType)) {
			return false;
		}

		// for selectors this is [0, maxParamNameIndex]
		if (min != other.min) {
			return false;
		}

		if (max != other.max) {
			return false;
		}

		if (stepSize != other.stepSize) {
			return false;
		}

		if (value != other.value) {
			return false;
		}

		if (rangeMin != other.rangeMin) {
			return false;
		}

		if (rangeMax != other.rangeMax) {
			return false;
		}

		if (label != other.label) {
			return false;
		}

		if (options.size() != other.options.size()) {
			return false;
		}

		return true;
	}

	friend class Device;
	friend class Modulation;
	friend class Preamp;
	friend class Poweramp;
	friend class Pedal;
	friend class Reverb;
	friend class Delay;
};

