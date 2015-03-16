#include "TerrainClass.h"

Vector3 TerrainClass::START_TERRAIN_POSITION = Vector3(0, 0, 0);
float TerrainClass::SCALE_TERRAIN = 50.0f;
bool TerrainClass::RESTART_TERRAIN = false;
float TerrainClass::LINEAR_VELOCITY_Z = -0.3f;
bool TerrainClass::HAS_COLLIDED_WITH_TERRAIN = false;
Vector3 TerrainClass::LINEAR_VELOCITY_TERRAIN = Vector3(0, 0, -0.3f);
bool TerrainClass::UPDATE_SERVER = false;

TerrainClass::TerrainClass() {
	srand(time(NULL));
	//create the terrain
	terrain = new ChunckManager(SCALE_TERRAIN);
	treshold = 0 - ((SCALE_TERRAIN * VoxelChunck::getZChunckSize()) - START_TERRAIN_POSITION.z);
	countTerrainEntities = 0;
	debug = false;

	BuildFloorEntity();

	amountMovementZ = -terrainPhysicsNodes.at(0)->GetAmountMovement().z;
	backupAmountMovementZ = 0.0f;
}

//restarts the terrain
void TerrainClass::restartTerrain() {
	amountMovementZ = -terrainPhysicsNodes.at(0)->GetAmountMovement().z;
	backupAmountMovementZ = 0.0f;
	treshold = 0 - ((SCALE_TERRAIN * VoxelChunck::getZChunckSize()) - START_TERRAIN_POSITION.z);

	for (int i = 0; i < terrainPhysicsNodes.size(); i++) {
		terrainPhysicsNodes.at(i)->SetPosition(START_TERRAIN_POSITION);
	}
	terrain->restartChunckManager();
}

TerrainClass::~TerrainClass() {
	terrainChuncksId.clear();
	terrainEntities.clear();
	terrainPhysicsNodes.clear();
}

void TerrainClass::updateLinearVelocityTerrain() {
	for (int i = 0; i < terrainChuncksId.size(); i++) {
		terrainPhysicsNodes.at(i)->SetLinearVelocity(LINEAR_VELOCITY_TERRAIN);
	}

	resetVelocityOfPuddleCHunck(LINEAR_VELOCITY_TERRAIN);
}

void TerrainClass::resetVelocityOfPuddleCHunck(Vector3 velocity) {
	for (int i = 0; i < ChunckManager::NUM_PUDDLE_CHUNCKS; i++) {
		puddlePhysicsNodes.at(i)->SetLinearVelocity(velocity);
	}
}

void TerrainClass::UpdateTerrain() {
	//update linear velocity of terrain
	updateLinearVelocityTerrain();

	// Purpose: Update the track
	if (terrain && terrainEntities.size() > 0) {
		//if the player is past the end of the first chunck then delete the chunck he is at and add a new one at the end
		PhysicsNode* terrainPhysics_01 = terrainPhysicsNodes.at(1);
		Vector3 nr_01 = terrainPhysics_01->GetPosition();

		if ((nr_01.z <= treshold)) {
			//create the chunck at the end
			terrain->createChunck();
			treshold -= (VoxelChunck::getZChunckSize() * SCALE_TERRAIN);
			float diff = abs(amountMovementZ) - abs(backupAmountMovementZ);
			amountMovementZ = -diff;

			UPDATE_SERVER = true;

			//reset puddle positions in world
			for (int i = 0; i < ChunckManager::NUM_PUDDLE_CHUNCKS; i++) {
				puddlePhysicsNodes.at(i)->SetPosition(terrain->getPositionOfOnePuddle(i));
			}
		}
	}
	backupAmountMovementZ = amountMovementZ;

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_R) && Window::GetKeyboard()->KeyHeld(KEYBOARD_R)) {
		restartTerrain();
	}
}

void TerrainClass::ResetTerrainVelocityAfterCollision() {
	for (int i = 0; i < terrainChuncksId.size(); i++) {
		terrainPhysicsNodes.at(i)->SetLinearVelocity(Vector3(0, 0, LINEAR_VELOCITY_Z));
		LINEAR_VELOCITY_TERRAIN = Vector3(0, 0, LINEAR_VELOCITY_Z);
	}
}

float TerrainClass::GetAmountMovementOnZAxis() {
	//recalculate the amount of movement made by the player on the terrain
	Vector3 amountMovement = terrainPhysicsNodes.at(0)->GetAmountMovement();
	amountMovementZ = amountMovementZ + amountMovement.z;
	return amountMovementZ;
}

bool TerrainClass::GetActiveVoxel(Vector3 position) {
	return terrain->getVoxelActive(position);
}

void TerrainClass::SetMovementTerrain(Vector3 position) {
	for (int i = 0; i < terrainChuncksId.size(); i++) {
		terrainPhysicsNodes.at(i)->SetPosition(position);

		//disable applying linear velocity this frame
		terrainPhysicsNodes.at(i)->SetLinearVelocity(Vector3(0, 0, 0));
		LINEAR_VELOCITY_TERRAIN = Vector3(0, 0, 0);
	}
}

Vector3 TerrainClass::GetTerrainPositionAt(int i) {
	return terrainPhysicsNodes.at(i)->GetPosition();
}

void TerrainClass::UpdatePhysicsTerrain(float msec) {
	//update the physics nodes
	for (int i = 0; i < terrainPhysicsNodes.size(); i++) {
		terrainPhysicsNodes.at(i)->Update(msec);
	}

	//update trees
	for (int i = 0; i < puddlePhysicsNodes.size(); i++) {
		puddlePhysicsNodes.at(i)->Update(msec);
	}
}

void TerrainClass::BuildFloorEntity() {
	for (int i = 0; i < ChunckManager::NUM_VOXEL_CHUNCKS; i++) {
		SceneNode* sNode = new SceneNode(terrain->getMeshAt(i));
		sNode->SetModelScale(Vector3(SCALE_TERRAIN, SCALE_TERRAIN, SCALE_TERRAIN));

		PhysicsNode* pNode = new PhysicsNode();
		pNode->SetPosition(START_TERRAIN_POSITION);
		pNode->SetLinearVelocity(LINEAR_VELOCITY_TERRAIN);
		pNode->SetUseLinearDamping(false);
		pNode->SetUseGravity(false);
		terrainPhysicsNodes.push_back(pNode);
		countTerrainEntities++;

		GameEntity* geFirst = new GameEntity(sNode, pNode);
		geFirst->SetConnectToPhysicsSystem(false);
		geFirst->ConnectToSystems();
		terrainEntities.push_back(geFirst);

		//add the id of the chunck just created
		terrainChuncksId.push_back(geFirst->getId());
		//reset the chunck id to be the same as the entity added; for consistency purposes
		terrain->getChunckAt(i)->setId(i);
	}

	for (int i = 0; i < ChunckManager::NUM_PUDDLE_CHUNCKS; i++) {
		SceneNode* sNode = new SceneNode(terrain->getPuddleChunckAt(i));
		sNode->SetModelScale(Vector3(50, 50, 50));
		sNode->SetTransform(Matrix4::Translation(terrain->getPuddlePositionAt(i)));

		PhysicsNode* pNode = new PhysicsNode();
		pNode->SetLinearVelocity(LINEAR_VELOCITY_TERRAIN);
		pNode->SetUseLinearDamping(false);
		pNode->SetUseGravity(false);
		pNode->SetPosition(sNode->GetTransform().GetPositionVector());
		puddlePhysicsNodes.push_back(pNode);

		GameEntity* ge = new GameEntity(sNode, pNode);
		ge->SetConnectToPhysicsSystem(false);
		ge->ConnectToSystems();
		//set the id of the puddle
		puddleIds.push_back(countTerrainEntities);
		countTerrainEntities++;

		terrainEntities.push_back(ge);
	}
}

float TerrainClass::getHeightAtPosition(Vector3 position) {
	return terrain->getHeightAtPosition(position, SCALE_TERRAIN);
}
