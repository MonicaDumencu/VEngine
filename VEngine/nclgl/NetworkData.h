#pragma once

#include <string.h>

#define MAX_PACKET_SIZE 1000000
#define PACKET_SIZE 256

enum PacketTypes {
	INIT_CONNECTION = 0,
	INIT_SERVER_TO_CLIENT_DATA,
	PLAYER_STATUS,
	SERVER_TO_CLIENT_UPDATE_TERRAIN_DATA,
	NOT_ALL_CLIENTS_CONNECTED,
	PLAYER_INFO,
	END_OF_CHUNCK_EVENT
};


struct Packet {
	PacketTypes packet_type;
	int packet_length;
	char data[PACKET_SIZE];

	void serialize(char* dArr) {
		memcpy(dArr, this, sizeof(Packet));
	}

	void deserialize(char* dArr) {
		memcpy(this, dArr, sizeof(Packet));
	}
};
