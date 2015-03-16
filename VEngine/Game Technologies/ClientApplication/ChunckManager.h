#pragma once

#include "VoxelChunck.h"
#include "SphereChunck.h"
#include "PuddleChunck.h"
#include "../../libnoise/include/noise/noise.h"
#include "noiseutils.h"

class ChunckManager {
public:
	static int NUM_VOXEL_CHUNCKS;
	static int NUM_PUDDLE_CHUNCKS;

	ChunckManager(float scale);
	~ChunckManager();

	void createChunck();

	Mesh* getMeshAt(int i);
	Mesh* getMeshWithId(int id);
	Mesh* getPuddleChunckAt(int id);
	VoxelChunck* getChunckAt(int i);

	int getSizeOfChuncks() const { return voxelChuncks.size(); }
	int getMaxHeightTerrain() const;

	Vector3 getPositionLastChunck() const { return POSITION_LAST_CHUNCK; }
	Vector3 getPuddlePositionAt(int i) { return puddleStartPositions.at(i); }
	Vector3 getPositionOfOnePuddle(int i);
	Mesh* getPuddleChunckAt(int i) const { return puddleChuncks.at(i)->getCube(); }

	bool getVoxelActive(Vector3 position) const;
	void restartChunckManager();
	void setScaleTerrain(int scale) { scaleTerrain = scale; }
	void setPositionOfPuddleChuncks();

	float getHeightAtPosition(Vector3 position, float scaleTerrain);

	static Vector3 transformVoxelCoordinatesToWorldCoordinates(Vector3 position, float scale, int xOffset, int yOffset, int zOffset);
	static Vector3 transformWorldCoordinatesToVoxelCoordinates(Vector3 position, float scale, int xOffset, int yOffset, int zOffset);

	static vector<Vector3> POSITIONS_PUDDLES;

	BlockType getBlockTypeAt(int x, int y, int z) const { return voxelChuncks.at(0)->getBlockTypeAt(x, y, z); }
private:
	vector<VoxelChunck*> voxelChuncks;
	vector<PuddleChunck*> puddleChuncks;
	vector<Vector3> puddleStartPositions;
	vector<Vector3> puddleSizes;

	Vector3 positionToPlaceAnotherChunck;

	GLuint texture;
	GLuint bumpTexture;
	
	vector<GLuint> puddleTextures;

	int xOffset;
	int zOffset;
	int scaleTerrain;
	module::Perlin perlinModule;

	Vector3 POSITION_LAST_CHUNCK;
};