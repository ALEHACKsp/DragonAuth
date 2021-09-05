// include the auth header
#include "auth.h"

// include native header files
#include <string>
#include <sstream>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <process.h>

// other headers
#include "socket.h"

// vars
ClientModule socketSrv;
bool ponged = false;

// concat 2 strings
const char* concat(const char* string1, const char* string2) {
	char* result = new char[strlen(string1) + strlen(string2) + 1];
	strcpy_s(result, strlen(string1) + strlen(string2) + 1, string1);
	strcat_s(result, strlen(string1) + strlen(string2) + 1, string2);
	return result;
}

// log prefix thingy
const char* getLogPrefix() {
	time_t currentTime;
	struct tm localTime;

	time(&currentTime);

	localtime_s(&localTime, &currentTime);

	std::stringstream ss;
	ss << "[" << localTime.tm_hour << ":" << localTime.tm_min << ":" << localTime.tm_sec << "] [DragonAuth] ";

	return ss.str().c_str();
};

// pParams are from _beginthread, see last argument
void runRecvThread(void* pParams) {
	std::string message = socketSrv.recvMsgs();
	while (message != "") {
		std::string delimiter = dragon_auth::SIGN_OFF;
		size_t pos = 0;
		std::string token;
		while ((pos = message.find(delimiter)) != std::string::npos) {
			token = message.substr(0, pos);
			message.erase(0, pos + delimiter.length());
			if(dragon_auth::verbose) std::cout << "token: " << token << std::endl;
		}
		message = socketSrv.recvMsgs();
	}
}

void connect_socket() {
	socketSrv.connectTo((char*)"127.0.0.1");
	// Ovious design
	if (!socketSrv.isOnline())
	{
		MessageBox(NULL, "Failed to initialize (1).", "dude, error.", MB_OK | MB_ICONERROR);
		exit(0);
	}

	// displays messages received from the server
	_beginthread(runRecvThread, 0, NULL);

	socketSrv.sendMessage((char*)concat("initialization/ping", dragon_auth::SIGN_OFF));
}

namespace dragon_auth {
	void disc() {
		socketSrv.sendMessage((char*)concat("socket/close", SIGN_OFF));
	}

	void init() {
		if(verbose) std::cout << getLogPrefix() << "Verbose enabled." << std::endl;

		connect_socket();
	};

	bool authenticate() {
		socketSrv.sendMessage((char*)concat("auth/init", SIGN_OFF));

		return true;
	}
}
