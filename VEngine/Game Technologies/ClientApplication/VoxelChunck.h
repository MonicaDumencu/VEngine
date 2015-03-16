#pragma once

#include "VoxelCube.h"
#include "../../nclgl/Mesh.h"
#include "../../nclgl/SceneNode.h"
#include "GameEntity.h"
#include "PhysicsNode.h"
#include "../../libnoise/include/noise/noise.h"
#include "noiseutils.h"
#include <thread>
#include <mutex>

class VoxelChunck {
public:
	VoxelChunck() {}
	VoxelChunck(GLuint tex, GLuint btexture, Vector3 start, int id, float xOffset, float yOffset, module::Perlin perlinModule);
	~VoxelChunck();

	Mesh* getCube() { return cube; }

	Vector3 getEndOffset() const { return endOffset; }
	void setEndOffset(Vector3 vect) { endOffset = vect; }

	BlockType getBlockTypeAt(int x, int y, int z) const { return blocks[x][y][z].getBlockType(); }

	Vector3 getStartOffset() const { return startOffset; }
	void setStartOffset(Vector3 st) { startOffset = st; }

	int getId() const { return id; }
	void setId(int val) { id = val; }

	int getMaxHeightTerrain() const { return maxHeightTerrain; }

	void createMesh();
	void updateMesh();
	void createCube(int x, int y, int z, bool onLeft, bool onRight, bool onTop, bool onBottom, bool onFront, bool onBack);

	void updateChunck(GLuint texture, GLuint bTexture, Vector3 position, float xOffset, float yOffset, module::Perlin perlinModule);

	void setupLandscape(module::Perlin perlinModule, float xOffset, float yOffset);

	bool getIsVoxelActive(Vector3 position) const;
	void setTextureForCube(GLuint texture, GLuint bumpTexture);
	float getHeightAtPosition(Vector3 position, module::Perlin module, float scaleTerrain);

	static int getXChunckSize() { return CHUNCK_SIZE_X; }
	static int getYChunckSize() { return CHUNCK_SIZE_Y; }
	static int getZChunckSize() { return CHUNCK_SIZE_Z; }

protected:
	VoxelCube*** blocks;
	Mesh* cube;
	GLuint texture;
	GLuint bumpTexture;

	Vector4 rectangleBoundaries;
	int id;
	int maxHeightTerrain;

	Vector3 startOffset;
	Vector3 endOffset;

	Vector3* arrayVertices;
	Vector2* arrayTexture;
	Vector3* arrayNormals;
	Vector3* arrayTangents;

private:
	static int CHUNCK_SIZE_X;
	static int CHUNCK_SIZE_Y;
	static int CHUNCK_SIZE_Z;

	static const vector<Vector2> grassTextureCoordinates;
	static const vector<Vector2> sandTextureCoordinates;
	static const vector<Vector2> bareGroundTextureCoordinates;
	static const vector<Vector2> rockTextureCoordinates;

	int rand1;

	void createVerticesAndTextureCoordinates(int x, int y, int z, bool onLeft, bool onRight, bool onTop, bool onBottom, bool onFront, bool onBack, const vector<Vector2> *textures);
	int transformHeightToBlockCoordinates(int height);
};