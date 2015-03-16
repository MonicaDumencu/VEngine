#include "TerrainGenerationData.h"

TerrainGenerationData::TerrainGenerationData() {
	int x = rand() % 100;
	int z = rand() % 500;

	offsetInNoiseMap.x = x;
	offsetInNoiseMap.y = 0;
	offsetInNoiseMap.z = z;

	NUM_TREES = rand() % 10;

	treeSizes = new int[NUM_TREES];
	treePositions = new Vector3[NUM_TREES];

	//initialize the tree Sizes and positions
	for (int i = 0; i < NUM_TREES; i++) {
		int randNum = rand() % 10 + 10;				//bigger trees
		treeSizes[i] = (randNum);
		setPositionTreeAt(i);
	}

	//set the initial spawner position
	//set it to 0 for now, more difficult otherwise (TO DO)
	spawnerPosition = Vector3(0, 0, 0);
}

TerrainGenerationData::~TerrainGenerationData() {
	delete treePositions;
	delete treeSizes;
}

Vector3 TerrainGenerationData::setPositionTreeAt(int i) {
	int x = rand() % 100;
	if (x > 50) {
		x = CHUNCK_SIZE_X - 6;
	}
	else {
		x = -9;
	}

	int y = 26.0f;
	int z = (rand() % (int)treeSizes[i]) + (rand() % 200);
	treePositions[i] = Vector3(x * SCALE_TERRAIN, y, z * SCALE_TERRAIN);
	return treePositions[i];
}

//will be called at the creation of a new chunck
void TerrainGenerationData::update() {
	//update the heightmap coordinates
	offsetInNoiseMap.x += CHUNCK_SIZE_X;
	offsetInNoiseMap.z += CHUNCK_SIZE_Z;

	//update the trees position
	for (int i = 0; i < NUM_TREES; i++) {
		setPositionTreeAt(i);
	}

	//here will be the update for spawner object
}