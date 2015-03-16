#include "MyGame.h"

MyGame::MyGame()	{
	gameCamera = new Camera();
	gameCamera->SetPosition(Vector3(0, 450, 0));
	gameCamera->SetPitch(-45.0);
	gameCamera->SetYaw(180.0);

	Renderer::GetRenderer().SetCamera(gameCamera);

	// create the player //
	player = new PlayerClass();
	player->setPlayerId(player->getPlayerEntity()->getId());
	allEntities.push_back(player->getPlayerEntity());

	// ------- Added by Monica ------ //
	//create the terrain
	terrain = new TerrainClass();
	player->setTerrainEntity(terrain);

	debug = true;

	//add the terrain nodes to allEntities
	for (int i = 0; i < terrain->GetTerrainEntitiesSize(); i++) {
		allEntities.push_back(terrain->GetGameEntityAt(i));
	}

	//set the entity in physics system
	// needed to update the player 
	PhysicsSystem::GetPhysicsSystem().SetPlayerEntity(player);
	PhysicsSystem::GetPhysicsSystem().SetPlayerPhysicsNode(player->getPlayerPhysicsNode());
	//set the terrain in physicsSystem
	PhysicsSystem::GetPhysicsSystem().SetTerrainEntity(terrain);
	// ---------------------------------------------------- //
}

MyGame::~MyGame(void)	{
}


void MyGame::UpdateGame(float msec) {
	if(gameCamera) {
		gameCamera->UpdateCamera(msec);

		//// --------------- Added by Monica ----------------------- //
		terrain->UpdateTerrain();
		//// ---------------------------------------------------- //
	}

	for(vector<GameEntity*>::iterator i = allEntities.begin(); i != allEntities.end(); ++i) {
		(*i)->Update(msec);
	}
}
