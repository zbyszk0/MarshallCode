#pragma once

#include <fstream>
#include <mutex>

class Log {
	static Log *instance;
	std::ofstream log;
	std::mutex mutex;

	Log() {
		log.open("log.txt");
	}

	~Log() {
		if (log.is_open()) {
			log.flush();
			log.close();
		}
	}
public:
	static Log *Get() {
		if (instance == nullptr) {
			instance = new Log();
		}

		return instance;
	}

	void Write(std::string message) {
		try {
			std::lock_guard<std::mutex> lock(mutex);
			log.write(message.c_str(), message.length());
			log.flush();
		}
		catch (...) {}
	}

	void Shutdown() {
		if (instance) {
			delete instance;
			instance = nullptr;
		}
	}
};
