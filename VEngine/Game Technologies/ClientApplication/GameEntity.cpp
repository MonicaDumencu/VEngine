#include "GameEntity.h"
#include "Renderer.h"
#include "PhysicsSystem.h"

// -------- Added by Monica ---- //
int GameEntity::newId = 0;
// --------------------------- //

GameEntity::GameEntity(bool val) : id(newId) {
	renderNode = NULL;
	physicsNode = NULL;

	// -------- Added by Monica ---- //
	if (!val) {
		newId++;
	}
	connectToPhysicsSystem = true;
	// --------------------------- //
}

GameEntity::GameEntity(SceneNode* s, PhysicsNode* p, bool val) : id(newId) {
	renderNode = s;
	physicsNode = p;

	// -------- Added by Monica ---- //
	if (!val) {
		newId++;
	}
	connectToPhysicsSystem = true;
	// --------------------------- //
}

GameEntity::~GameEntity(void)	{
	DisconnectFromSystems();

	delete renderNode;
	delete physicsNode;
}

void	GameEntity::Update(float msec) {

}

void	GameEntity::ConnectToSystems() {
	if(renderNode) {
		Renderer::GetRenderer().AddNode(renderNode);
	}

	if(physicsNode) {
		PhysicsSystem::GetPhysicsSystem().AddNode(physicsNode);
	}

	if(renderNode && physicsNode) {
		physicsNode->SetTarget(renderNode);
	}
}

void	GameEntity::DisconnectFromSystems() {
	if(renderNode) {
		Renderer::GetRenderer().RemoveNode(renderNode);
	}

	if(physicsNode) {
		PhysicsSystem::GetPhysicsSystem().RemoveNode(physicsNode);
	}
}