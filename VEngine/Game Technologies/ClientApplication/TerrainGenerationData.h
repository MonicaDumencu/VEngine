#pragma once

#include <vector>
#include "..\..\nclgl\Vector3.h"

class TerrainGenerationData {
public:
	TerrainGenerationData();
	~TerrainGenerationData();

	void update();

	Vector3 getOffsetInNoiseMap() const { return offsetInNoiseMap; }
	Vector3 getTreePositionAt(int i) const { return treePositions[i]; }
	Vector3 getSpanwerPosition() const { return spawnerPosition; }
	Vector3* getTreePositions()const { return treePositions; };
	int* getTreeSizes() const { return treeSizes; };
	int getTreeSizesAt(int i) const { return treeSizes[i]; }
	int getNumTrees() const{ return NUM_TREES; }
	int getNumVoxelChunks() const { return NUM_VOXEL_CHUNCKS; }
	int getChunckSizeX() const { return CHUNCK_SIZE_X; }
	int getChunckSizeY() const { return CHUNCK_SIZE_Y; }
	int getChunckSizeZ() const { return CHUNCK_SIZE_Z; }
	int getTerrainScale() const { return SCALE_TERRAIN; }

	void setOffsetInNoiseMap(Vector3 vect) { offsetInNoiseMap = vect; }
	void setTreePositionAt(Vector3 vect, int i) { treePositions[i] = vect; }
	void setSpawnerPosition(Vector3 vect) { spawnerPosition = vect; }
	void setTreePostions(Vector3* data) { treePositions = data; }
	void setTreeSizes(int* data) { treeSizes = data; }
	void setNumTrees(int num) { NUM_TREES = num; }
	void setNumVoxelChuncks(int num) { NUM_VOXEL_CHUNCKS = num; }
	void setChunckSizeX(int num) { CHUNCK_SIZE_X = num; }
	void setChunckSizeY(int num) { CHUNCK_SIZE_Y = num; }
	void setChunckSizeZ(int num) { CHUNCK_SIZE_Z = num; }
	void setScaleTerrain(int num) { SCALE_TERRAIN = num; }

	Vector3 setPositionTreeAt(int i);

private:
	Vector3 offsetInNoiseMap;
	Vector3* treePositions;
	int* treeSizes;
	Vector3 spawnerPosition;

	int NUM_TREES;
	int NUM_VOXEL_CHUNCKS;

	int CHUNCK_SIZE_X;
	int CHUNCK_SIZE_Y;
	int CHUNCK_SIZE_Z;

	int SCALE_TERRAIN;
};