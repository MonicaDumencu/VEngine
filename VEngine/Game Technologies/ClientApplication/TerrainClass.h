#pragma once

#include "ChunckManager.h"

class TerrainClass {
public:
	TerrainClass();
	~TerrainClass();

	void UpdateTerrain();
	void UpdatePhysicsTerrain(float msec);
	//build the floor entities
	void BuildFloorEntity();

	int GetTerrainEntitiesSize() const { return terrainEntities.size(); }
	int getMaxHeightTerrain() const { return terrain->getMaxHeightTerrain() * SCALE_TERRAIN; }
	Vector3 getPositionLastChunck() const { return terrain->getPositionLastChunck() * SCALE_TERRAIN; }

	GameEntity* GetGameEntityAt(int i) { return terrainEntities.at(i); }
	PhysicsNode* GetTerrainPhysicsNodeAt(int i) { return terrainPhysicsNodes.at(i); }

	// ----- Collision detection methods ----- //
	void ResetTerrainVelocityAfterCollision();
	float GetAmountMovementOnZAxis();
	bool GetActiveVoxel(Vector3 position);
	void SetMovementTerrain(Vector3 position);
	Vector3 GetTerrainPositionAt(int i);
	void SetAmountMovementz(float val) { amountMovementZ = val; }
	Vector3 GetVelocityOfPuddleChunck() { return puddlePhysicsNodes.at(0)->GetLinearVelocity(); }
	Vector3 GetVelocityOfTerrain() { return terrainPhysicsNodes.at(0)->GetLinearVelocity(); }
	void UpdateVelocityOfTrees() { resetVelocityOfPuddleCHunck(terrainPhysicsNodes.at(0)->GetLinearVelocity()); }

	BlockType getBlockTypeAt(Vector3 position) const { return terrain->getBlockTypeAt(position.x, position.y, position.z); }
	/// ------------------------------------- ///

	void restartTerrain();
	void resetVelocityOfPuddleCHunck(Vector3 velocity);
	float getHeightAtPosition(Vector3 position);
	//added by Matrix
	void SetLinearVelocityZ(float value){ LINEAR_VELOCITY_Z = value; }

	static Vector3 START_TERRAIN_POSITION;
	static Vector3 LINEAR_VELOCITY_TERRAIN;
	static float SCALE_TERRAIN;
	static bool RESTART_TERRAIN;
	static float LINEAR_VELOCITY_Z;
	static bool HAS_COLLIDED_WITH_TERRAIN;
	static bool UPDATE_SERVER;
private:
	void updateLinearVelocityTerrain();

	ChunckManager* terrain;

	vector<int> terrainChuncksId;
	vector<int> puddleIds;
	vector<GameEntity*> terrainEntities;
	vector<PhysicsNode*> terrainPhysicsNodes;
	vector<PhysicsNode*> puddlePhysicsNodes;

	float treshold;
	float amountMovementZ;
	float backupAmountMovementZ;

	int countTerrainEntities;

	bool debug;
};