#pragma once

#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include <stdio.h> 

#include "..\..\nclgl\NetworkData.h"
#include "..\..\nclgl\NetworkServices.h"

#define SERVER_IP_ADDRESS "10.66.66.67"
// size of our buffer
#define DEFAULT_BUFLEN 512
// port to connect sockets through 
#define DEFAULT_PORT "6881"
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class ClientNetwork {
public:
	int iResult;
	SOCKET ConnectSocket;

	ClientNetwork();
	~ClientNetwork();

	int receivePackets(char * recvbuf);
};