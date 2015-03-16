#include "SphereChunck.h"

//int SphereChunck::CHUNCK_SIZE_X = 4;
//int SphereChunck::CHUNCK_SIZE_Y = 4;
//int SphereChunck::CHUNCK_SIZE_Z = 4;

SphereChunck::SphereChunck(GLuint tex, GLuint btexture, Vector3 start, int id) {
	CHUNCK_SIZE_X = 4;
	CHUNCK_SIZE_Y = 4;
	CHUNCK_SIZE_Z = 4;

	blocks = new VoxelCube**[(const int)CHUNCK_SIZE_X];
	for (int i = 0; i < CHUNCK_SIZE_X; i++) {
		blocks[i] = new VoxelCube*[(const int)CHUNCK_SIZE_Y];
		for (int j = 0; j < CHUNCK_SIZE_Y; j++) {
			blocks[i][j] = new VoxelCube[(const int)CHUNCK_SIZE_Z];
		}
	}

	cube = Mesh::GenerateCube();
	setTextureForCube(tex, btexture);
	this->id = id;

	arrayVertices = new Vector3[36];
	arrayTexture = new Vector2[36];
	arrayNormals = new Vector3[36];
	arrayTangents = new Vector3[36];

	//bind buffer data
	cube->BindBuffers(CHUNCK_SIZE_X * CHUNCK_SIZE_Y * CHUNCK_SIZE_Z * 36);
	setupLandscape();
	createMesh();
	cube->UpdateMeshData();
}

SphereChunck::~SphereChunck() {
	for (int i = 0; i < CHUNCK_SIZE_X; i++) {
		for (int j = 0; j < CHUNCK_SIZE_Y; j++) {
			delete[] blocks[i][j];
		}
		delete[] blocks[i];
	}
	delete[] blocks;
	delete cube;

	delete[] arrayVertices;
	delete[] arrayTexture;
}

void SphereChunck::setTextureForCube(GLuint texture, GLuint bumpTexture) {
	cube->SetTexture(texture);
	cube->SetBumpMap(bumpTexture);
	cube->SetTextureAndBumpmapRepeat(true);
}

void SphereChunck::setupLandscape() {
	for (int x = 0; x < CHUNCK_SIZE_X; x++) {
		for (int z = 0; z < CHUNCK_SIZE_Z; z++) {
			blocks[x][0][z].setIsActive(true);
		}
	}
}

void SphereChunck::createMesh() {
	for (int i = 0; i < CHUNCK_SIZE_X; i++) {
		for (int j = 0; j < CHUNCK_SIZE_Y; j++) {
			for (int k = 0; k < CHUNCK_SIZE_Z; k++) {
				if (blocks[i][j][k].getIsActive()) {
					bool onLeft = false;
					bool onRight = false;
					bool onTop = false;
					bool onBottom = false;
					bool onFront = false;
					bool onBack = false;
					if (i > 0) {
						onLeft = blocks[i - 1][j][k].getIsActive();
					}
					if (i < CHUNCK_SIZE_X - 1) {
						onRight = blocks[i + 1][j][k].getIsActive();
					}
					if (j > 0) {
						onBottom = blocks[i][j - 1][k].getIsActive();
					}
					if (j < CHUNCK_SIZE_Y - 1) {
						onTop = blocks[i][j + 1][k].getIsActive();
					}
					if (k > 0) {
						onBack = blocks[i][j][k - 1].getIsActive();
					}
					if (k < CHUNCK_SIZE_Z - 1) {
						onFront = blocks[i][j][k + 1].getIsActive();
					}
					//create the cube at the position according to the offset
					createCube(i, j, k, onLeft, onRight, onTop, onBottom, onFront, onBack);
				}
			}
		}
	}
}

void SphereChunck::createCube(int x, int y, int z, bool onLeft, bool onRight, bool onTop, bool onBottom, bool onFront, bool onBack) {
	int nrVertices = (!onLeft) * 6 + (!onRight) * 6 + (!onTop) * 6 + (!onBottom) * 6 + (!onFront) * 6 + (!onBack) * 6;

	Vector3 p1(x - VoxelCube::BLOCK_RENDER_SIZE, y - VoxelCube::BLOCK_RENDER_SIZE, z + VoxelCube::BLOCK_RENDER_SIZE);
	Vector3 p2(x + VoxelCube::BLOCK_RENDER_SIZE, y - VoxelCube::BLOCK_RENDER_SIZE, z + VoxelCube::BLOCK_RENDER_SIZE);
	Vector3 p3(x + VoxelCube::BLOCK_RENDER_SIZE, y + VoxelCube::BLOCK_RENDER_SIZE, z + VoxelCube::BLOCK_RENDER_SIZE);
	Vector3 p4(x - VoxelCube::BLOCK_RENDER_SIZE, y + VoxelCube::BLOCK_RENDER_SIZE, z + VoxelCube::BLOCK_RENDER_SIZE);
	Vector3 p5(x + VoxelCube::BLOCK_RENDER_SIZE, y - VoxelCube::BLOCK_RENDER_SIZE, z - VoxelCube::BLOCK_RENDER_SIZE);
	Vector3 p6(x - VoxelCube::BLOCK_RENDER_SIZE, y - VoxelCube::BLOCK_RENDER_SIZE, z - VoxelCube::BLOCK_RENDER_SIZE);
	Vector3 p7(x - VoxelCube::BLOCK_RENDER_SIZE, y + VoxelCube::BLOCK_RENDER_SIZE, z - VoxelCube::BLOCK_RENDER_SIZE);
	Vector3 p8(x + VoxelCube::BLOCK_RENDER_SIZE, y + VoxelCube::BLOCK_RENDER_SIZE, z - VoxelCube::BLOCK_RENDER_SIZE);

	auto createNormalsAndTangents = [&](const int& currentIndex){

		const int indexA = currentIndex - 6;
		const int indexB = currentIndex - 5;
		const int indexC = currentIndex - 4;

		//create normals here
		Vector3 &a = arrayVertices[indexA];
		Vector3 &b = arrayVertices[indexB];
		Vector3 &c = arrayVertices[indexC];

		Vector3 normal = Vector3::Cross(b - a, c - a);
		normal.Normalise();

		for (int k = 1; k < 7; ++k)
		{
			arrayNormals[currentIndex - k] = normal;
		}

		//create tangents here
		Vector2 coord1 = arrayTexture[indexB] - arrayTexture[indexA];
		Vector2 coord2 = arrayTexture[indexC] - arrayTexture[indexA];

		Vector3 vertex1 = arrayVertices[indexB] - arrayVertices[indexA];
		Vector3 vertex2 = arrayVertices[indexC] - arrayVertices[indexA];

		Vector3 axis = Vector3(vertex1*coord2.y - vertex2*coord1.y);

		float factor = 1.0f / (coord1.x * coord2.y - coord2.x * coord1.y);
		Vector3 tangent = Mesh::GenerateTangent(arrayVertices[indexA], arrayVertices[indexB], arrayVertices[indexC], arrayTexture[indexA], arrayTexture[indexB], arrayTexture[indexC]);

		for (int k = 1; k < 7; ++k)
		{
			arrayTangents[currentIndex - k] = tangent;
		}
	};

	int index = 0;
	if (!onFront) {
		arrayTexture[index] = Vector2(0.0f, 0.0f); arrayVertices[index++] = p1; arrayTexture[index] = Vector2(1.0f, 0.0f);	arrayVertices[index++] = p2; arrayTexture[index] = Vector2(1.0f, 1.0f);	arrayVertices[index++] = p3;
		arrayTexture[index] = Vector2(1.0f, 1.0f); arrayVertices[index++] = p3; arrayTexture[index] = Vector2(0.0f, 1.0f);	arrayVertices[index++] = p4; arrayTexture[index] = Vector2(0.0f, 0.0f);	arrayVertices[index++] = p1;
		createNormalsAndTangents(index);
	}
	if (!onBack) {
		arrayTexture[index] = Vector2(0.0f, 0.0f); arrayVertices[index++] = p7; arrayTexture[index] = Vector2(1.0f, 0.0f);	arrayVertices[index++] = p8; arrayTexture[index] = Vector2(1.0f, 1.0f);	arrayVertices[index++] = p5;
		arrayTexture[index] = Vector2(1.0f, 1.0f); arrayVertices[index++] = p5; arrayTexture[index] = Vector2(0.0f, 1.0f);	arrayVertices[index++] = p6; arrayTexture[index] = Vector2(0.0f, 0.0f);	arrayVertices[index++] = p7;
		createNormalsAndTangents(index);
	}
	if (!onTop) {
		arrayTexture[index] = Vector2(0.0f, 0.0f); arrayVertices[index++] = p4; arrayTexture[index] = Vector2(1.0f, 0.0f);	arrayVertices[index++] = p3; arrayTexture[index] = Vector2(1.0f, 1.0f);	arrayVertices[index++] = p8;
		arrayTexture[index] = Vector2(1.0f, 1.0f); arrayVertices[index++] = p8; arrayTexture[index] = Vector2(0.0f, 1.0f);	arrayVertices[index++] = p7; arrayTexture[index] = Vector2(0.0f, 0.0f);	arrayVertices[index++] = p4;
		createNormalsAndTangents(index);
	}
	if (!onBottom) {
		arrayTexture[index] = Vector2(0.0f, 0.0f); arrayVertices[index++] = p6; arrayTexture[index] = Vector2(1.0f, 0.0f);	arrayVertices[index++] = p5; arrayTexture[index] = Vector2(1.0f, 1.0f);	arrayVertices[index++] = p2;
		arrayTexture[index] = Vector2(1.0f, 1.0f); arrayVertices[index++] = p2; arrayTexture[index] = Vector2(0.0f, 1.0f);	arrayVertices[index++] = p1; arrayTexture[index] = Vector2(0.0f, 0.0f);	arrayVertices[index++] = p6;
		createNormalsAndTangents(index);
	}
	if (!onLeft) {
		arrayTexture[index] = Vector2(0.0f, 0.0f); arrayVertices[index++] = p6; arrayTexture[index] = Vector2(1.0f, 0.0f);	arrayVertices[index++] = p1; arrayTexture[index] = Vector2(1.0f, 1.0f);	arrayVertices[index++] = p4;
		arrayTexture[index] = Vector2(1.0f, 1.0f); arrayVertices[index++] = p4; arrayTexture[index] = Vector2(0.0f, 1.0f);	arrayVertices[index++] = p7; arrayTexture[index] = Vector2(0.0f, 0.0f);	arrayVertices[index++] = p6;
		createNormalsAndTangents(index);
	}
	if (!onRight) {
		arrayTexture[index] = Vector2(0.0f, 0.0f); arrayVertices[index++] = p2; arrayTexture[index] = Vector2(1.0f, 0.0f);	arrayVertices[index++] = p5; arrayTexture[index] = Vector2(1.0f, 1.0f);	arrayVertices[index++] = p8;
		arrayTexture[index] = Vector2(1.0f, 1.0f); arrayVertices[index++] = p8; arrayTexture[index] = Vector2(0.0f, 1.0f);	arrayVertices[index++] = p3; arrayTexture[index] = Vector2(0.0f, 0.0f);	arrayVertices[index++] = p2;
		createNormalsAndTangents(index);
	}

	cube->AddCubeToMesh(arrayVertices, arrayTexture, arrayNormals, arrayTangents, nrVertices);
}

bool SphereChunck::getIsVoxelActive(Vector3 position) const {
	return blocks[(int)position.x][(int)position.y][(int)position.z].getIsActive();
}