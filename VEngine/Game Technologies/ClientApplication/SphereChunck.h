#pragma once

#include "VoxelCube.h"
#include "VoxelChunck.h"

class SphereChunck {
public:
	SphereChunck(GLuint tex, GLuint btexture, Vector3 start, int id);
	SphereChunck() {}
	~SphereChunck();

	virtual void setupLandscape();

	Mesh* getCube() { return cube; }

	int getId() const { return id; }
	void setId(int val) { id = val; }

	void createMesh();
	void createCube(int x, int y, int z, bool onLeft, bool onRight, bool onTop, bool onBottom, bool onFront, bool onBack);

	void setTextureForCube(GLuint texture, GLuint bumpTexture);
	bool getIsVoxelActive(Vector3 position) const;

	int getXChunckSize() { return CHUNCK_SIZE_X; }
	int getYChunckSize() { return CHUNCK_SIZE_Y; }
	int getZChunckSize() { return CHUNCK_SIZE_Z; }

private:
	int CHUNCK_SIZE_X;
	int CHUNCK_SIZE_Y;
	int CHUNCK_SIZE_Z;

	int id;

	VoxelCube*** blocks;
	Mesh* cube;

	GLuint texture;
	GLuint bumpTexture;

	Vector3* arrayVertices;
	Vector2* arrayTexture;
	Vector3* arrayNormals;
	Vector3* arrayTangents;
};