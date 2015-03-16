/*
Author: Monica
Purpose: Creates the player entity and contains everything that involves the player in the game(animation, collision detection etc)
Modified: Matrix (robot animation)
*/


#pragma once

#include "../../nclgl/Mesh.h"
#include "GameEntity.h"
#include "ChunckManager.h"
#include "TerrainClass.h"
#include"Renderer.h"

#define LINEAR_VELOCITY_GRASS -0.3f
#define LINEAR_VELOCITY_BARE_GROUND -0.4f
#define LINEAR_VELOCITY_ROCK -0.1f
#define LINEAR_VELOCITY_SAND  -0.2f

static float TotalIncreaseValueofTerrain = 0.0f;
class PlayerClass {
public:
	PlayerClass();
	~PlayerClass();

	// ------ Added by Monica ---------- //
	void UpdatePlayer(float msec);

	int getPlayerId() const { return playerId; }
	void setPlayerId(int id) { playerId = id; }

	float getAmountHorizontaLinearVelocity() const { return amountHorizontalLinearVelocity; }
	void setAmountHorizontalLinearVelocity(float nr) { amountHorizontalLinearVelocity = nr; }

	float getAmountMovementOnZAxis() const { return amountMovementZ; }

	Vector3 getPlayerCoordinatesVoxel() const { return playerCoordinatesVoxel; }
	void setPlayerCoordinatesVoxel(Vector3 vect) { playerCoordinatesVoxel = vect; }

	bool setPlayerVoxelCoordinates(Vector3 position);
	Vector3 getPlayerCoordinatesFromVoxel(bool left, bool right);

	GameEntity* getPlayerEntity() const { return playerEntity; }
	PhysicsNode* getPlayerPhysicsNode() const { return physicsNodePlayer; }

	static bool getGoLeft() { return GO_LEFT; }
	static bool getGoRight() { return GO_RIGHT; }
	static bool getjump() { return JUMP; }
	static Vector3 getVelocityTerrain() { return VELOCITY_TERRAIN; }

	void setTerrainEntity(TerrainClass* terr) { terrain = terr; }
	void resetPlayerOnTerrain(Vector3 position, float scale);
	void checkPlayerOnTerrainBounds(Vector3 position);

	void makePlayerFallOffTerrain();

	void detectAndResolveCollisionsWithTerrain();
	// -------------------------------------- //

	void BuildRobotEntity();
	static Vector3 PLAYER_POSITION;

private:
	//-------  Added by Monica ------- //
	int playerId;
	float amountHorizontalLinearVelocity;
	float amountMovementZ;
	Vector3 playerCoordinatesVoxel;

	GameEntity* playerEntity;
	PhysicsNode* physicsNodePlayer;
	SceneNode*   renderNodePlayer;
	TerrainClass* terrain;
	Mesh* cube;

	static bool GO_LEFT;
	static bool GO_RIGHT;
	static bool JUMP;
	static Vector3 VELOCITY_TERRAIN;
	static bool HAS_FALLEN_OFF_TERRAIN;
	bool isVelocityFalse;
	bool debug;

	// --- Private methods for collision detection --- //
	void resetTerrainVelocity();
	void positionPlayerOnTerrain();
	Vector3 updatePlayersMovementOnTerrain();

	bool checkAndResolvePlayerCollisionOnYAxis(Vector3 playerPosition, Vector3 position, Vector3 velocity);
	bool checkAndResolvePlayerCollisionOnXAxisLeft(Vector3 playerPosition, Vector3 position, Vector3 velocity);
	bool checkAndResolvePlayerCollisionOnXAxisRight(Vector3 playerPosition, Vector3 position, Vector3 velocity);
	void checkAndResolvePlayerCollisionOnZAxis(Vector3 playerPosition, Vector3 terrainPosition);
	// -------------------------------- //
};