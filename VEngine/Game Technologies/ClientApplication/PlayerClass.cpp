#include "PlayerCLass.h"
#include "..\..\nclgl\OBJMesh.h"
#include "..\..\nclgl\CubeRobot.h"
#include <math.h>

Vector3 PlayerClass::PLAYER_POSITION = Vector3(400, 1000, TerrainClass::START_TERRAIN_POSITION.z);
bool PlayerClass::GO_LEFT = false;
bool PlayerClass::GO_RIGHT = false;
bool PlayerClass::JUMP = false;
bool PlayerClass::HAS_FALLEN_OFF_TERRAIN = false;
Vector3 PlayerClass::VELOCITY_TERRAIN = Vector3(0, 0, 0);

PlayerClass::PlayerClass() {
	CubeRobot::CreateCube();

	BuildRobotEntity();
	amountHorizontalLinearVelocity = 0.001f;

	isVelocityFalse = false;
	debug = false;
}

PlayerClass::~PlayerClass() {
	delete playerEntity;
}

void PlayerClass::UpdatePlayer(float msec) {
	/// ------ Added by Monica ------ //
	TerrainClass::HAS_COLLIDED_WITH_TERRAIN = false;
	Vector3 posPlayer = physicsNodePlayer->GetPosition();

	//// --------- Added by Monica --------------------------------//
	//detectAndResolveCollisionsWithTerrain();
	//// ---------------------------------------------------------------- //

	//######### DEBUG ###########
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_T))	{
		debug = !debug;
	}
	//###############################
}

void PlayerClass::resetTerrainVelocity() {
	//reset the velocity of the terrain when the player has collided with an object and then moved
	if (GO_RIGHT || GO_LEFT || JUMP) {
		GO_LEFT = false;
		GO_RIGHT = false;
		JUMP = false;

		isVelocityFalse = false;
		terrain->ResetTerrainVelocityAfterCollision();
	}
}

void PlayerClass::positionPlayerOnTerrain() {
	Vector3 oldPositionPlayer = physicsNodePlayer->GetPosition();
	float scale = TerrainClass::SCALE_TERRAIN;
	if (oldPositionPlayer.y < ((scale / 2) + 1)) {
		oldPositionPlayer.y = ((scale / 2) + 1);
		physicsNodePlayer->SetPosition(oldPositionPlayer);
	}
}

Vector3 PlayerClass::updatePlayersMovementOnTerrain() {
	//update the players position with how much the terrain has moved since last time
	Vector3 playerPosition = physicsNodePlayer->GetPosition();
	amountMovementZ = terrain->GetAmountMovementOnZAxis();

	//add the z movement of the terrain
	playerPosition.z -= amountMovementZ;
	return playerPosition;
}

bool PlayerClass::checkAndResolvePlayerCollisionOnYAxis(Vector3 playerPosition, Vector3 position, Vector3 velocity) {
		Vector3 transformCoord = getPlayerCoordinatesFromVoxel(false, false);

	if (round(playerPosition.y) >= round(transformCoord.y)) {
		position.y = transformCoord.y + 26.0f;

		physicsNodePlayer->SetPosition(position);
		physicsNodePlayer->SetLinearVelocity(Vector3(velocity.x, 0, velocity.z));

		playerPosition = physicsNodePlayer->GetPosition();
		playerPosition.z -= amountMovementZ;
		return setPlayerVoxelCoordinates(playerPosition);
	}
	return true;
}

bool PlayerClass::checkAndResolvePlayerCollisionOnXAxisLeft(Vector3 playerPosition, Vector3 position, Vector3 velocity) {
	Vector3 transformCoord = getPlayerCoordinatesFromVoxel(false, false);

	if ((round(playerPosition.x) > (transformCoord.x + 25.0f))) {
		Vector3 normal = playerPosition - transformCoord;
		//get the distance squared
		float distSq = Vector3::Dot(normal, normal);
		//calculate the distance between the origins of the 2 objects
		float distance = 5.0f + 25.0f;
		//calculate penetration depth
		float penetration = distance - sqrtf(distSq);

		position.x += abs(penetration);
		//position.x = transformCoord.x + 30.0f;

		physicsNodePlayer->SetLinearVelocity(Vector3(0, velocity.y, velocity.z));
		physicsNodePlayer->SetPosition(position);

		playerPosition = physicsNodePlayer->GetPosition();
		playerPosition.z -= amountMovementZ;

		return setPlayerVoxelCoordinates(playerPosition);
	}
	return true;
}

bool PlayerClass::checkAndResolvePlayerCollisionOnXAxisRight(Vector3 playerPosition, Vector3 position, Vector3 velocity) {
	Vector3 transformCoord = getPlayerCoordinatesFromVoxel(false, false);

	if ((round(playerPosition.x) < (transformCoord.x - 25.0f))) {
			Vector3 normal = playerPosition - transformCoord;
			//get the distance squared
			float distSq = Vector3::Dot(normal, normal);
			//calculate the distance between the origins of the 2 objects
			float distance = 5.0f + 25.0f;
			//calculate penetration depth
			float penetration = distance - sqrtf(distSq);

			//offset it a little so that the collision does not happen that often
			position.x -= abs(penetration);

			physicsNodePlayer->SetPosition(position);
			physicsNodePlayer->SetLinearVelocity(Vector3(0, velocity.y, velocity.z));

			playerPosition = physicsNodePlayer->GetPosition();
			playerPosition.z -= amountMovementZ;

			return setPlayerVoxelCoordinates(playerPosition);
	}
	return true;
}

void PlayerClass::checkAndResolvePlayerCollisionOnZAxis(Vector3 playerPosition, Vector3 terrainPosition) {
	Vector3 transformCoord = getPlayerCoordinatesFromVoxel(false, false);

	if ((round(playerPosition.z) <= (transformCoord.z))) {
		Vector3 normal = playerPosition - transformCoord;
		//get the distance squared
		float distSq = Vector3::Dot(normal, normal);
		//calculate the distance between the origins of the 2 objects
		float distance = 5.0f + 27.0f;
		//calculate penetration depth
		float penetration = distance - sqrtf(distSq);

		if (penetration > 0.0f) {
			penetration = -10.0f;
		}

		terrainPosition.z -= (penetration);
		terrain->SetMovementTerrain(terrainPosition);
		isVelocityFalse = true;
		amountMovementZ = amountMovementZ - (penetration);
		terrain->SetAmountMovementz(amountMovementZ);
	}
}

void PlayerClass::detectAndResolveCollisionsWithTerrain() {
	//// ------------- Collision detection with terrain ------------------ //
	resetTerrainVelocity();
	Vector3 playerPosition = updatePlayersMovementOnTerrain();

	bool value = setPlayerVoxelCoordinates(playerPosition);

	//check to see if the player is within the bounds of the terrain
	checkPlayerOnTerrainBounds(playerPosition);

	Vector3 terrPos_00 = terrain->GetTerrainPositionAt(0);
	Vector3 position = physicsNodePlayer->GetPosition();
	Vector3 velocity = physicsNodePlayer->GetLinearVelocity();

	//######### DEBUG ###########
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_T))	{
		debug = !debug;
	}
	//###############################

	//get the type of the block that the player is on and change the velocity accordingly
	BlockType bType = terrain->getBlockTypeAt(playerCoordinatesVoxel);
	switch (bType) {
		case BlockType_Bare: {
			TerrainClass::LINEAR_VELOCITY_Z = LINEAR_VELOCITY_BARE_GROUND + TotalIncreaseValueofTerrain;
			break;
		}
		case BlockType_Rock: {
			TerrainClass::LINEAR_VELOCITY_Z = LINEAR_VELOCITY_ROCK + TotalIncreaseValueofTerrain;
			break;
		}
		case BlockType_Sand: {
			TerrainClass::LINEAR_VELOCITY_Z = LINEAR_VELOCITY_SAND + TotalIncreaseValueofTerrain;
			break;
		}
		default: {
			TerrainClass::LINEAR_VELOCITY_Z = LINEAR_VELOCITY_GRASS + TotalIncreaseValueofTerrain;
			break;
		}
	}

	////the actual collision detection
	////check the collision with objects coming from ahead
	if (value == true) {
		TerrainClass::HAS_COLLIDED_WITH_TERRAIN = true;
		value = checkAndResolvePlayerCollisionOnYAxis(playerPosition, position, velocity);
	}

	if (value == true) {
		TerrainClass::HAS_COLLIDED_WITH_TERRAIN = true;
		value = checkAndResolvePlayerCollisionOnXAxisLeft(playerPosition, position, velocity);
	}

	if (value == true) {
		TerrainClass::HAS_COLLIDED_WITH_TERRAIN = true;
		value = checkAndResolvePlayerCollisionOnXAxisRight(playerPosition, position, velocity);
	}

	if (value == true) {
		TerrainClass::HAS_COLLIDED_WITH_TERRAIN = true;
		checkAndResolvePlayerCollisionOnZAxis(playerPosition, terrPos_00);
	}
}

void PlayerClass::resetPlayerOnTerrain(Vector3 position, float scale) {
	//get the height of the terrain at the position of the player and respawn it on the terrain with the coorect height
	float height = terrain->getHeightAtPosition(Vector3((VoxelChunck::getXChunckSize() - 1) * scale / 2, position.y, position.z));
	physicsNodePlayer->SetPosition(Vector3((VoxelChunck::getXChunckSize() - 1) * scale / 2, height, 0.0f));
	//reset the velocity of the player
	physicsNodePlayer->SetLinearVelocity(Vector3(0, 0, 0));
}

void PlayerClass::checkPlayerOnTerrainBounds(Vector3 position) {
	Vector3 playerPos = physicsNodePlayer->GetPosition();
	float scale = TerrainClass::SCALE_TERRAIN;
	if (playerPos.x >(VoxelChunck::getXChunckSize() - 1) * scale || playerPos.x < 0) {
		//resetPlayerOnTerrain(position, scale);
		makePlayerFallOffTerrain();
	}

	//for the y axis so that the player does not get outside the range
	if (playerPos.y > (VoxelChunck::getYChunckSize() - 2) * scale) {
		physicsNodePlayer->SetPosition(Vector3(playerPos.x, (VoxelChunck::getYChunckSize() - 2) * scale, playerPos.z));
	}
}

void PlayerClass::makePlayerFallOffTerrain() {
	//make the player stop the update
	HAS_FALLEN_OFF_TERRAIN = true;

	//apply gravity to player
	physicsNodePlayer->SetUseGravity(true);
}


void PlayerClass::BuildRobotEntity() {
	renderNodePlayer = new CubeRobot();
	renderNodePlayer->SetModelScale(Vector3(30, 40, 30));
	renderNodePlayer->SetTransform(Matrix4::Translation(PLAYER_POSITION));
	
	physicsNodePlayer = new PhysicsNode();
	physicsNodePlayer->SetPosition(PLAYER_POSITION);

	physicsNodePlayer->SetUseGravity(true);

	playerEntity = new GameEntity(NULL, physicsNodePlayer);
	playerEntity->SetConnectToPhysicsSystem(true);
	playerEntity->ConnectToSystems();
}

/// -------- Added by Monica ------ //
bool PlayerClass::setPlayerVoxelCoordinates(Vector3 position) {
	float scale = TerrainClass::SCALE_TERRAIN;
	//for the robot the offset will be on the x axis +- 40
	Vector3 posXPositive = ChunckManager::transformWorldCoordinatesToVoxelCoordinates(position, scale, 10, 0, 0);
	Vector3 posXNegative = ChunckManager::transformWorldCoordinatesToVoxelCoordinates(position, scale, -10, 0, 0);

	if (posXPositive.z >= 50) {
		posXPositive.z = 49;
	}
	if (posXNegative.z >= 50) {
		posXNegative.z = 49;
	}

	if (terrain->GetActiveVoxel(posXPositive)) {
		//set the player voxel coordinates
		playerCoordinatesVoxel = posXPositive;
		return true;
	}
	if (terrain->GetActiveVoxel(posXNegative)) {
		//set the player voxel coordinates
		playerCoordinatesVoxel = posXNegative;
		return true;
	}
	return false;
}

Vector3 PlayerClass::getPlayerCoordinatesFromVoxel(bool left, bool right) {
	float scale = TerrainClass::SCALE_TERRAIN;
	if (left) {
		return ChunckManager::transformVoxelCoordinatesToWorldCoordinates(playerCoordinatesVoxel, scale, -10, 0, 0);
	}
	if (right) {
		return ChunckManager::transformVoxelCoordinatesToWorldCoordinates(playerCoordinatesVoxel, scale, 10, 0, 0);
	}
	return ChunckManager::transformVoxelCoordinatesToWorldCoordinates(playerCoordinatesVoxel, scale, 0, 0, 0);
}
/// ------------------------------------- ///