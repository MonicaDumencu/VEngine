#pragma once

#include "..\..\nclgl\Vector3.h"

class PlayerDataNetworking {
public:
	PlayerDataNetworking();
	~PlayerDataNetworking();

	void setPlayerPosition(Vector3 pos) { playerPosition = pos; }
	void setHealth(float hel) { health = hel; }
	void setTimeLeft(float time) { timeLeft = time; }
	void setScore(float sc) { score = sc; }
	void setPlayerId(unsigned int id) { playerId = id; }

	float GetPlayerHealth(){ return health; };
	float GetPlayertimeLeft(){ return timeLeft; };
	float GetPlayerScore(){ return score; };

private:
	Vector3 playerPosition;
	float health;
	float timeLeft;
	float score;

	unsigned int playerId;
};