#include "NetworkServices.h"

int NetworkServices::sendMessage(SOCKET currentSocket, char* message, int messageSize) {
	return send(currentSocket, message, messageSize, 0);
}

int NetworkServices::receiveMessage(SOCKET currentSocket, char* buffer, int bufferSize) {
	return recv(currentSocket, buffer, bufferSize, 0);
}