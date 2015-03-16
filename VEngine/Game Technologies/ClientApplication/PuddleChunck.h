#pragma once

#include "SphereChunck.h"

class PuddleChunck {
public:
	PuddleChunck(GLuint tex, GLuint btexture, Vector3 start, int id, int xStart, int yStart, int zStart);
	~PuddleChunck();

	void setupLandscape();

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
	int CHUNCK_SIZE_Z;
	int CHUNCK_SIZE_Y;
	int CHUNCK_SIZE_X;

	int id;

	VoxelCube*** blocks;
	Mesh* cube;

	GLuint texture;
	GLuint bumpTexture;

	Vector3* arrayVertices;
	Vector2* arrayTexture;
	Vector3* arrayNormals;
	Vector3* arrayTangents;

	static const vector<Vector2> grassTextureCoordinates;
	static const vector<Vector2> sandTextureCoordinates;
	static const vector<Vector2> bareGroundTextureCoordinates;
	static const vector<Vector2> rockTextureCoordinates;

	void createVerticesAndTextureCoordinates(int x, int y, int z, bool onLeft, bool onRight, bool onTop, bool onBottom, bool onFront, bool onBack, const vector<Vector2> *textures);
};