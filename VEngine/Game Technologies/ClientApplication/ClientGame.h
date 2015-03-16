#pragma once

#include <winsock2.h>
#include <Windows.h>
#include "ClientNetwork.h"
#include "..\..\nclgl\NetworkData.h"
#include <thread>
#include "../../nclgl/Window.h"
#include "../../nclgl/SoundSystem.h"
#include "ClientGame.h"
#include <process.h>
#include "MyGame.h"
#include "HUDManager.h"
#include "TerrainGenerationData.h"
#include "PlayerDataNetworking.h"
#include "PlayerInformationNetworking.h"

class ClientGame { 
public: 
	ClientGame();
	~ClientGame(void);

	ClientNetwork* network;

	void sendActionPackets(PacketTypes TYPE);
	char network_data[MAX_PACKET_SIZE];

	int InitializeWindow();
	void InitializeGameSinglePlayer();
	void UpdateSinglePlayer();
	void RunGame();
	int TerminateGame();
	int Quit(bool pause = false, const string &reason = "");
	void physicsLoop(GameClass* game, bool& running);

	void InitializeGameMultiPlayer();
	void UpdateMultiPlayer();

	static bool NEED_UPDATE;

protected:
	MyGame* game;
	bool running;
	std::thread physics;
	bool justOnceInitPacket;

	bool notAllClientsConnected;
	static bool PLAY_GAME;
	static bool UPDATE_PLAYER_STATUS;

	int numOfTrees;
	PlayerDataNetworking* pDNetworking;
	TerrainGenerationData* terrainData;

	void DecodeInitSToCData(const Packet& packet, TerrainGenerationData* terrainData);
	void DecodeSToCUpdateData(const Packet& packet, Vector3* treePositions, Vector3& spawnerPosition, const int& numTrees);
	Packet* EncodeCToSPlayerData(const PlayerDataNetworking* playerData);
	void DecodeSToCPlayerInfoData(const Packet& packet);
};