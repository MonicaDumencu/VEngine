#pragma once

#include "..\..\nclgl\Vector3.h"
#include "PlayerDataNetworking.h"

class PlayerInformationNetworking {
public:
	static void Initialise() { instance = new PlayerInformationNetworking(); }
	static void Destroy() { delete instance; }
	static PlayerInformationNetworking& GetPlayerInformationNetworking() { return *instance; }

	PlayerDataNetworking* GetPlayerDataNetworking(){ return pDNet; };
	int GetNumPlayers(){ return numPlayers; };
	void setPlayerDataNetworking(PlayerDataNetworking* pNet) { 
		/*if (pDNet) 
			delete pDNet;*/ 
		pDNet = pNet; 
	}

private:
	PlayerInformationNetworking();
	~PlayerInformationNetworking();

	static PlayerInformationNetworking* instance;

	//how many players there are and their data
	int numPlayers;
	PlayerDataNetworking* pDNet;
};