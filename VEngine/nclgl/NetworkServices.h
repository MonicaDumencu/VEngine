#pragma once

#include <WinSock2.h>
#include <Windows.h>

class NetworkServices {
public:
	static int sendMessage(SOCKET currentSocket, char* messahe, int messageSize);
	static int receiveMessage(SOCKET currentSocket, char* buffer, int bufferSize);
};