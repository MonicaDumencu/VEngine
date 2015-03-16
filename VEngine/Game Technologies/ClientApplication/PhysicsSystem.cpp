#include "PhysicsSystem.h"

PhysicsSystem* PhysicsSystem::instance = 0;

PhysicsSystem::PhysicsSystem(void)	{

}

PhysicsSystem::~PhysicsSystem(void)	{

}

void	PhysicsSystem::Update(float msec) {	
	allNodesMutex.lock();
	copiedNodes.clear();
	copiedNodes = allNodes;
	allNodesMutex.unlock();

	for (vector<PhysicsNode*>::iterator i = copiedNodes.begin(); i != copiedNodes.end(); ++i) {
		//if something is deleted in all nodes, just skip this frames update..... or not?
		//TODO: evaluate if the skipping is necessary ---- Kan
		if (copiedNodes.size() != allNodes.size())
			break;
		(*i)->Update(msec);
	}

	// -------- Added by Monica ----- //
	//now update the player's physicsnode
	//this was necessary because the threading caused problems in the normal update in mygame class
	//it is important that the player update will take place before the terrain one
	player->UpdatePlayer(msec);
	playerPhysics->Update(msec);

	terrain->UpdatePhysicsTerrain(msec);
	// ------------------------------- //
}

void	PhysicsSystem::AddNode(PhysicsNode* n) {
	allNodesMutex.lock();
	allNodes.push_back(n);
	allNodesMutex.unlock();
}

void	PhysicsSystem::RemoveNode(PhysicsNode* n) {
	for(vector<PhysicsNode*>::iterator i = allNodes.begin(); i != allNodes.end(); ++i) {
		if((*i) == n) {
			allNodesMutex.lock();
			allNodes.erase(i);
			allNodesMutex.unlock();
			return;
		}
	}
}