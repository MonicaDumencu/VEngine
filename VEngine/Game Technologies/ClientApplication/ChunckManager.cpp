#include "ChunckManager.h"

int ChunckManager::NUM_VOXEL_CHUNCKS = 3;
int ChunckManager::NUM_PUDDLE_CHUNCKS = 5;

vector<Vector3> ChunckManager::POSITIONS_PUDDLES = { Vector3(0, 100, 0), Vector3(500, 0, 0), Vector3(200, 0, 200) };

ChunckManager::ChunckManager(float scale) : scaleTerrain(scale){
	//random terrain
	xOffset = rand() % 100;
	zOffset = rand() % 500;

	//perlinModule.SetFrequency(5.0);

	//createHeightMapForLandscape();
	texture = SOIL_load_OGL_texture(TEXTUREDIR"texture.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	bumpTexture = SOIL_load_OGL_texture(TEXTUREDIR"GRASS_NORMAL.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	for (int i = 0; i < NUM_PUDDLE_CHUNCKS; i = i + 2) {
		puddleTextures.push_back(SOIL_load_OGL_texture(TEXTUREDIR"tree_tex_01.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		puddleTextures.push_back(SOIL_load_OGL_texture(TEXTUREDIR"tree_tex_02.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	}

	Vector3 start(0, 0, 0);

	int i;
	for (i = 0; i < NUM_VOXEL_CHUNCKS; i++) {
		VoxelChunck* vChunck;
		vChunck = new VoxelChunck(texture, bumpTexture, start, i, xOffset, zOffset, perlinModule);
		//set the starting point for the next chunck of terrain
		start = vChunck->getEndOffset();
		voxelChuncks.push_back(vChunck);

		//update the coordinates for the heightmap
		xOffset += VoxelChunck::getXChunckSize();
		zOffset += VoxelChunck::getZChunckSize();

	}
	positionToPlaceAnotherChunck = start;
	POSITION_LAST_CHUNCK = Vector3(VoxelChunck::getXChunckSize() / 2, 0, voxelChuncks.at(NUM_VOXEL_CHUNCKS - 2)->getEndOffset().z);

	//creation of voxel trees

	//initialize the puddle Sizes and positions
	for (int i = 0; i < NUM_PUDDLE_CHUNCKS; i++) {
		int randNum = rand() % 10 + 10;				//bigger trees
		puddleSizes.push_back(Vector3(randNum, randNum, randNum));
		puddleStartPositions.push_back(getPositionOfOnePuddle(i));

		//initialize the puddles
		puddleChuncks.push_back(new PuddleChunck(puddleTextures.at(i), puddleTextures.at(i), puddleStartPositions.at(i), i, puddleSizes.at(i).x, puddleSizes.at(i).y, puddleSizes.at(i).z));
	}
}

Vector3 ChunckManager::getPositionOfOnePuddle(int i) {
	int x = rand() % (VoxelChunck::getXChunckSize() - 2);
	int y = rand() % (VoxelChunck::getYChunckSize() - 2);
	int z = rand() % 150;

	//get an offset for the tree position (since the trunk is situated in more than one voxel - diagonal)
	float height_1 = voxelChuncks.at(0)->getHeightAtPosition(Vector3(x, y, z), perlinModule, scaleTerrain) * scaleTerrain;
	float height_2 = voxelChuncks.at(0)->getHeightAtPosition(Vector3(x, y, z + 1), perlinModule, scaleTerrain) * scaleTerrain;
	float height_3 = voxelChuncks.at(0)->getHeightAtPosition(Vector3(x + 1, y, z), perlinModule, scaleTerrain) * scaleTerrain;
	float height;

	//get on which chunk the tree will be placed
	if (z >= 50 && z < 100) {
		//for each chunk z will run from 0 to 50
		z -= 50;
		height_1 = voxelChuncks.at(1)->getHeightAtPosition(Vector3(x, y, z), perlinModule, scaleTerrain) * scaleTerrain;
		height_2 = voxelChuncks.at(1)->getHeightAtPosition(Vector3(x, y, z + 1), perlinModule, scaleTerrain) * scaleTerrain;
		height_3 = voxelChuncks.at(1)->getHeightAtPosition(Vector3(x + 1, y, z), perlinModule, scaleTerrain) * scaleTerrain;
		//put it back so that when it gets transformed to world coordinates it will be correct
		z += 50;
	}
	else {
		if (z >= 100 && z < 150) {
			z -= 100;
			height_1 = voxelChuncks.at(2)->getHeightAtPosition(Vector3(x, y, z), perlinModule, scaleTerrain) * scaleTerrain;
			height_2 = voxelChuncks.at(2)->getHeightAtPosition(Vector3(x, y, z + 1), perlinModule, scaleTerrain) * scaleTerrain;
			height_3 = voxelChuncks.at(2)->getHeightAtPosition(Vector3(x + 1, y, z), perlinModule, scaleTerrain) * scaleTerrain;
			z += 100;
		}
	}

	if (height_1 < 500 && height_2 < 500 && height_3 < 500) {
		height = min(min(height_1, height_2), height_3);
	}
	else {
		height = max(max(height_1, height_2), height_3);
	}
	return Vector3(x * scaleTerrain, height, z * scaleTerrain);
}

void ChunckManager::setPositionOfPuddleChuncks() {
	for (int i = 0; i < NUM_PUDDLE_CHUNCKS; i++) {
		puddleStartPositions.at(i) = getPositionOfOnePuddle(i);
	}
}

ChunckManager::~ChunckManager() {
	for (int i = 0; i < voxelChuncks.size(); i++) {
		delete voxelChuncks.at(i);
	}
	voxelChuncks.clear();
}

void ChunckManager::createChunck() {
	////the next chunck of terrain will be created at the end of the existing piece of terrain
	std::rotate(voxelChuncks.begin(), voxelChuncks.begin() + 1, voxelChuncks.end());

	//update the last voxel in the terrain to reflect the new terrain for the player to go on
	int index = voxelChuncks.size() - 1;
	voxelChuncks.at(index)->updateChunck(texture, bumpTexture, positionToPlaceAnotherChunck, xOffset, zOffset, perlinModule);

	//in case the offset gets too big
	if ((xOffset / VoxelChunck::getXChunckSize()) > 1000) {
		xOffset = 0.0f;
	}
	if ((zOffset / VoxelChunck::getZChunckSize()) > 1000) {
		zOffset = 0.0f;
	}

	//update the coordinates for the heightmap
	xOffset += VoxelChunck::getXChunckSize();
	zOffset += VoxelChunck::getZChunckSize();

	//set the starting point for the next chunck of terrain
	positionToPlaceAnotherChunck = voxelChuncks.at(voxelChuncks.size() - 1)->getEndOffset();
}

void ChunckManager::restartChunckManager() {
	xOffset = 0.0f;
	zOffset = 0.0f;
	Vector3 start(0, 0, 0);

	for (int i = 0; i < voxelChuncks.size(); i++) {
		voxelChuncks.at(i)->updateChunck(texture, bumpTexture, start, xOffset, zOffset, perlinModule);

		start = voxelChuncks.at(i)->getEndOffset();

		//update the coordinates for the heightmap
		xOffset += VoxelChunck::getXChunckSize();
		zOffset += VoxelChunck::getZChunckSize();
	}

	positionToPlaceAnotherChunck = start;

	//update the coordinates for the heightmap
	xOffset += VoxelChunck::getXChunckSize();
	zOffset += VoxelChunck::getZChunckSize();
	POSITION_LAST_CHUNCK = Vector3(VoxelChunck::getXChunckSize() / 2, 0, voxelChuncks.at(NUM_VOXEL_CHUNCKS - 2)->getEndOffset().z);
}

Mesh* ChunckManager::getMeshAt(int i) {
	return voxelChuncks.at(i)->getCube();
}

VoxelChunck* ChunckManager::getChunckAt(int i) {
	return voxelChuncks.at(i);
}

Mesh* ChunckManager::getMeshWithId(int id) {
	for (int i = 0; i < voxelChuncks.size(); i++) {
		if (voxelChuncks.at(i)->getId() == id) {
			return voxelChuncks.at(i)->getCube();
		}
	}
}

bool ChunckManager::getVoxelActive(Vector3 position) const {
	return voxelChuncks.at(0)->getIsVoxelActive(position);
}

float ChunckManager::getHeightAtPosition(Vector3 position, float scaleTerrain) {
	//transform the start position of the entity into voxel coordinates
	Vector3 voxels = transformWorldCoordinatesToVoxelCoordinates(position, scaleTerrain, 0, 0, 0);
	return voxelChuncks.at(voxelChuncks.size() - 1)->getHeightAtPosition(voxels, perlinModule, scaleTerrain);
}

Vector3 ChunckManager::transformVoxelCoordinatesToWorldCoordinates(Vector3 position, float scale, int xOffset = 0, int yOffset = 0, int zOffset = 0) {
	float x = position.x * scale + xOffset;
	float y = (position.y * scale) + yOffset;
	float z = (position.z * scale) + zOffset;

	return Vector3(x, y, z);
}

Vector3 ChunckManager::transformWorldCoordinatesToVoxelCoordinates(Vector3 position, float scale, int xOffset = 0, int yOffset = 0, int zOffset = 0) {
	int x = round((position.x + xOffset) / scale);
	int y = round((position.y + yOffset) / scale);
	int z = round(abs(position.z + zOffset) / scale);

	return Vector3(x, y, z);
}

int ChunckManager::getMaxHeightTerrain() const {
	int max = voxelChuncks.at(0)->getMaxHeightTerrain();
	for (int i = 1; i < voxelChuncks.size(); i++) {
		if (max < voxelChuncks.at(i)->getMaxHeightTerrain()) {
			max = voxelChuncks.at(i)->getMaxHeightTerrain();
		}
	}
	return max;
}

Mesh* ChunckManager::getPuddleChunckAt(int id) {
	return puddleChuncks.at(id)->getCube();
}