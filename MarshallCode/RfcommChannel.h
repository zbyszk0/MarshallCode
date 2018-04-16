#pragma once

#include <windows.h>
#include <Ws2bth.h>
#include <winsock.h>
#include <BluetoothAPIs.h>
#include <functional>
#include <ostream>
#include <iostream>
#include "Log.h"

typedef BOOL(*guidFromString_t)(LPCWSTR, LPGUID);

class RfcommChannel {
	static const int RECV_BUFFER_LEN = 1024;

	SOCKET s;
	HANDLE threadHandle;
	bool isConnected;
	char recvBuffer[RECV_BUFFER_LEN];

	std::function<void(char *, int)> receiveData;

private:
	GUID parseGuid(const wchar_t *guidString) {
		HMODULE shell32 = LoadLibraryW(TEXT("shell32.dll"));
		guidFromString_t guidFromString = (guidFromString_t)GetProcAddress(shell32, (LPCSTR)704);

		GUID sppGuid;
		if (guidFromString) {
			guidFromString(guidString, &sppGuid);
		}

		return sppGuid;
	}

	BTH_ADDR findDeviceAddress(const wchar_t* deviceName) {
		BTH_ADDR address = 0;

		BLUETOOTH_DEVICE_INFO_STRUCT deviceInfo;
		ZeroMemory(&deviceInfo, sizeof(deviceInfo));
		deviceInfo.dwSize = sizeof(deviceInfo);

		BLUETOOTH_DEVICE_SEARCH_PARAMS searchParams;
		ZeroMemory(&searchParams, sizeof(searchParams));
		searchParams.dwSize = sizeof(searchParams);
		searchParams.fReturnConnected = true;

		HBLUETOOTH_DEVICE_FIND findHandle = BluetoothFindFirstDevice(&searchParams, &deviceInfo);

		do {
			address = deviceInfo.Address.ullLong;
		} while (!wcsstr(deviceName, deviceInfo.szName) && BluetoothFindNextDevice(findHandle, &deviceInfo));

		BluetoothFindDeviceClose(findHandle);

		return address;
	}

	static unsigned long __stdcall threadProc(LPVOID userdata) {
		return reinterpret_cast<RfcommChannel *>(userdata)->run();
	}

public:
	RfcommChannel(std::function<void(char *, int)> receiveDataFn) {
		isConnected = false;
		s = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
		ZeroMemory(recvBuffer, sizeof(recvBuffer));
		threadHandle = nullptr;

		receiveData = receiveDataFn;
	}

	~RfcommChannel() {
		TerminateThread(threadHandle, 0);

		closesocket(s);
	}

	bool connect(const wchar_t *serviceId, const wchar_t *deviceName) {
		SOCKADDR_BTH address;
		address.serviceClassId = parseGuid(serviceId);
		address.addressFamily = AF_BTH;
		address.port = BT_PORT_ANY;
		address.btAddr = findDeviceAddress(deviceName);

		isConnected = ::connect(s, (sockaddr *)&address, sizeof(SOCKADDR_BTH)) != INVALID_SOCKET;

		if (isConnected) {
			start();
		}
		else {
			std::stringstream msg;
			msg << "Last error: " << WSAGetLastError() << std::endl;
			Log::Get()->Write(msg.str());
		}

		return (isConnected);
	}

	void start() {
		threadHandle = CreateThread(NULL, 0, &RfcommChannel::threadProc, this, 0, 0);

		if (!threadHandle) {
			std::stringstream msg;
			msg << "Last error: " << GetLastError() << std::endl;
			Log::Get()->Write(msg.str());
		}
	}

	int write(const unsigned char *buffer, int length) {
		return send(s, (char *)buffer, length, 0);
	}

	int run() {
		while (isConnected) {
			int len = 0;

			if ((len = recv(s, recvBuffer, RECV_BUFFER_LEN, 0)) > 0) {

				if (receiveData) {
					receiveData(recvBuffer, len);
				}
			}
		}

		return 0;
	}
};