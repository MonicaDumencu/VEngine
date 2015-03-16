#include "VoxelChunck.h"

using namespace noise;

int VoxelChunck::CHUNCK_SIZE_X = 60;
int VoxelChunck::CHUNCK_SIZE_Y = 50;
int VoxelChunck::CHUNCK_SIZE_Z = 50;

const vector<Vector2> VoxelChunck::grassTextureCoordinates = { Vector2(0.0f, 0.0f), Vector2(0.5f, 0.0f), Vector2(0.5f, 0.5f),
															   Vector2(0.5f, 0.5f), Vector2(0.0f, 0.5f), Vector2(0.0f, 0.0f) };
const vector<Vector2> VoxelChunck::sandTextureCoordinates = { Vector2(0.5f, 0.0f), Vector2(1.0f, 0.0f), Vector2(1.0f, 0.5f),
															  Vector2(1.0f, 0.5f), Vector2(0.5f, 0.5f), Vector2(0.5f, 0.0f) };
const vector<Vector2> VoxelChunck::bareGroundTextureCoordinates = { Vector2(0.0f, 0.5f), Vector2(0.5f, 0.5f), Vector2(0.5f, 1.0f),
																	Vector2(0.5f, 1.0f), Vector2(0.0f, 1.0f), Vector2(0.0f, 0.5f) };
const vector<Vector2> VoxelChunck::rockTextureCoordinates = { Vector2(0.5f, 0.5f), Vector2(1.0f, 0.5f), Vector2(1.0f, 1.0f),
															  Vector2(1.0f, 1.0f), Vector2(0.5f, 1.0f), Vector2(0.5f, 0.5f) };

VoxelChunck::VoxelChunck(GLuint tex, GLuint btexture, Vector3 start, int id, float xOffset, float yOffset, module::Perlin perlinModule) {
	startOffset = start;
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
	maxHeightTerrain = 25;

	arrayVertices = new Vector3[36];
	arrayTexture = new Vector2[36];
	arrayNormals = new Vector3[36];
	arrayTangents = new Vector3[36];

	//bind buffer data
	cube->BindBuffers(CHUNCK_SIZE_X * CHUNCK_SIZE_Y * CHUNCK_SIZE_Z * 36);
	setupLandscape(perlinModule, xOffset, yOffset);
	createMesh();
	cube->UpdateMeshData();
}

void VoxelChunck::setTextureForCube(GLuint texture, GLuint bumpTexture) {
	cube->SetTexture(texture);
	cube->SetBumpMap(bumpTexture);
	cube->SetTextureAndBumpmapRepeat(true);
}

VoxelChunck::~VoxelChunck() {
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

void VoxelChunck::createMesh() {
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
					createCube(i + startOffset.x, j, k + startOffset.z, onLeft, onRight, onTop, onBottom, onFront, onBack);
				}
			}
		}
	}

	//set the ending offset so that the next block knows where to start
	endOffset.x = startOffset.x;
	endOffset.y = startOffset.y;
	endOffset.z = startOffset.z + CHUNCK_SIZE_Z;
}

void VoxelChunck::createCube(int x, int y, int z, bool onLeft, bool onRight, bool onTop, bool onBottom, bool onFront, bool onBack) {
	int nrVertices = (!onLeft) * 6 + (!onRight) * 6 + (!onTop) * 6 + (!onBottom) * 6 + (!onFront) * 6 + (!onBack) * 6;

	int xBlocks = x - startOffset.x;
	int zBlocks = z - startOffset.z;
	switch (blocks[xBlocks][y][zBlocks].getBlockType()) {
		case BlockType_Rock: {
			createVerticesAndTextureCoordinates(x, y, z, onLeft, onRight, onTop, onBottom, onFront, onBack, &rockTextureCoordinates);
			break;
		}
		case BlockType_Bare: {
			createVerticesAndTextureCoordinates(x, y, z, onLeft, onRight, onTop, onBottom, onFront, onBack, &bareGroundTextureCoordinates);
			break;
		}
		case BlockType_Sand: {
			createVerticesAndTextureCoordinates(x, y, z, onLeft, onRight, onTop, onBottom, onFront, onBack, &sandTextureCoordinates);
			break;
		}
		default: {
			createVerticesAndTextureCoordinates(x, y, z, onLeft, onRight, onTop, onBottom, onFront, onBack, &grassTextureCoordinates);
			break;
		}
	}

	cube->AddCubeToMesh(arrayVertices, arrayTexture, arrayNormals, arrayTangents, nrVertices);
}

void VoxelChunck::createVerticesAndTextureCoordinates(int x, int y, int z, bool onLeft, bool onRight, bool onTop, bool onBottom, bool onFront, bool onBack, const vector<Vector2> *textures) {
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

		for (int k = 1; k < 7; ++k) {
			arrayTangents[currentIndex - k] = tangent;
		}
	};

	int index = 0;
	if (!onFront) {
		arrayTexture[index] = textures->at(0); arrayVertices[index++] = p1; arrayTexture[index] = textures->at(1);	arrayVertices[index++] = p2; arrayTexture[index] = textures->at(2);	arrayVertices[index++] = p3;
		arrayTexture[index] = textures->at(3); arrayVertices[index++] = p3; arrayTexture[index] = textures->at(4);	arrayVertices[index++] = p4; arrayTexture[index] = textures->at(5);	arrayVertices[index++] = p1;
		createNormalsAndTangents(index);
	}
	if (!onBack) {
		arrayTexture[index] = textures->at(0); arrayVertices[index++] = p7; arrayTexture[index] = textures->at(1);	arrayVertices[index++] = p8; arrayTexture[index] = textures->at(2);	arrayVertices[index++] = p5;
		arrayTexture[index] = textures->at(3); arrayVertices[index++] = p5; arrayTexture[index] = textures->at(4);	arrayVertices[index++] = p6; arrayTexture[index] = textures->at(5);	arrayVertices[index++] = p7;
		createNormalsAndTangents(index);
	}
	if (!onTop) {
		arrayTexture[index] = textures->at(0); arrayVertices[index++] = p4; arrayTexture[index] = textures->at(1);	arrayVertices[index++] = p3; arrayTexture[index] = textures->at(2);	arrayVertices[index++] = p8;
		arrayTexture[index] = textures->at(3); arrayVertices[index++] = p8; arrayTexture[index] = textures->at(4);	arrayVertices[index++] = p7; arrayTexture[index] = textures->at(5);	arrayVertices[index++] = p4;
		createNormalsAndTangents(index);
	}
	if (!onBottom) {
		arrayTexture[index] = textures->at(0); arrayVertices[index++] = p6; arrayTexture[index] = textures->at(1);	arrayVertices[index++] = p5; arrayTexture[index] = textures->at(2);	arrayVertices[index++] = p2;
		arrayTexture[index] = textures->at(3); arrayVertices[index++] = p2; arrayTexture[index] = textures->at(4);	arrayVertices[index++] = p1; arrayTexture[index] = textures->at(5);	arrayVertices[index++] = p6;
		createNormalsAndTangents(index);
	}
	if (!onLeft) {
		arrayTexture[index] = textures->at(0); arrayVertices[index++] = p6; arrayTexture[index] = textures->at(1);	arrayVertices[index++] = p1; arrayTexture[index] = textures->at(2);	arrayVertices[index++] = p4;
		arrayTexture[index] = textures->at(3); arrayVertices[index++] = p4; arrayTexture[index] = textures->at(4);	arrayVertices[index++] = p7; arrayTexture[index] = textures->at(5);	arrayVertices[index++] = p6;
		createNormalsAndTangents(index);
	}
	if (!onRight) {
		arrayTexture[index] = textures->at(0); arrayVertices[index++] = p2; arrayTexture[index] = textures->at(1);	arrayVertices[index++] = p5; arrayTexture[index] = textures->at(2);	arrayVertices[index++] = p8;
		arrayTexture[index] = textures->at(3); arrayVertices[index++] = p8; arrayTexture[index] = textures->at(4);	arrayVertices[index++] = p3; arrayTexture[index] = textures->at(5);	arrayVertices[index++] = p2;
		createNormalsAndTangents(index);
	}
}

void VoxelChunck::setupLandscape(module::Perlin perlinModule, float xOffset, float zOffset) {
	for (int x = 0; x < CHUNCK_SIZE_X; x++) {
		for (int y = 0; y < CHUNCK_SIZE_Y; y++) {
			for (int z = 0; z < CHUNCK_SIZE_Z; z++) {
				blocks[x][y][z].setIsActive(false);
				blocks[x][y][z].setBlockType(BlockType_Grass);
			}
		}
	}

	int x, z;
	for (z = 0; z < CHUNCK_SIZE_Z; z++) {
		for (x = 0; x < CHUNCK_SIZE_X; x++) {
			float nrX = (x)* 0.01f;
			float nrZ = (z + zOffset) * 0.01f;

			int height = (round((perlinModule.GetValue(nrX, 0, nrZ)) * 20)) + 15;

			//height control - voxels are up to 50 and can not drop below 0
			if (height > 49) {
				height = 49;
			}
			if (height < 0) {
				height = 0;
			}
			
			if (height <= 10) {
				for (int y = 0; y <= 10; y++) {
					blocks[x][y][z].setIsActive(false);
				}
				for (int y = 0; y <= height; y++) {
					blocks[x][y][z].setIsActive(true);
				}
			}
			else {
				for (int y = 10; y <= (height); y++) {
					if (height >= 10 && height < 20) {
						blocks[x][y][z].setBlockType(BlockType_Bare);
					}
					if (height >= 20 && height <= 25) {
						blocks[x][y][z].setBlockType(BlockType_Sand);
					}
					if (height > 25) {
						blocks[x][y][z].setBlockType(BlockType_Rock);
					}
					blocks[x][y][z].setIsActive(true);
				}
			}
		}
	}

	////uncomment this if you want to better adjust the velocity of terrain depending on type :D
	//for (int x = 0; x < CHUNCK_SIZE_X / 2; x++) {
	//	for (int z = 0; z < CHUNCK_SIZE_Z / 2; z++) {
	//		blocks[x][0][z].setIsActive(true);
	//		blocks[x][0][z].setBlockType(BlockType_Bare);
	//	}
	//	for (int z = CHUNCK_SIZE_Z / 2; z < CHUNCK_SIZE_Z; z++) {
	//		blocks[x][0][z].setIsActive(true);
	//		blocks[x][0][z].setBlockType(BlockType_Grass);
	//	}
	//}
	//for (int x = CHUNCK_SIZE_X / 2; x < CHUNCK_SIZE_X; x++) {
	//	for (int z = 0; z < CHUNCK_SIZE_Z / 2; z++) {
	//		blocks[x][0][z].setIsActive(true);
	//		blocks[x][0][z].setBlockType(BlockType_Rock);
	//	}
	//	for (int z = CHUNCK_SIZE_Z / 2; z < CHUNCK_SIZE_Z; z++) {
	//		blocks[x][0][z].setIsActive(true);
	//		blocks[x][0][z].setBlockType(BlockType_Sand);
	//	}
	//}

	//for (int i = 0; i < CHUNCK_SIZE_Z; i++) {
	//	blocks[0][1][i].setIsActive(true);
	//	blocks[CHUNCK_SIZE_Z - 1][1][i].setIsActive(true);
	//}
}

int VoxelChunck::transformHeightToBlockCoordinates(int height) {
	return height + 25;
}

float VoxelChunck::getHeightAtPosition(Vector3 position, module::Perlin module, float scaleTerrain) {
	for (int i = CHUNCK_SIZE_Y - 1; i >= 0; i--) {
		if (blocks[(int)position.x][i][(int)position.z].getIsActive() == true) {
			return (i);
		}
	}
}

void VoxelChunck::updateChunck(GLuint texture, GLuint bTexture, Vector3 position, float xOffset, float yOffset, module::Perlin perlinModule) {
	//i know the texture I want to set to the voxel
	this->texture = texture;
	this->bumpTexture = bTexture;
	//i know the position to set it at
	this->startOffset = position;

	//delete old data in mesh
	cube->DeleteOldData();

	//based on that I want to create the landscape that this mesh will represent
	//this will set the blocks within the mesh as active or not
	setupLandscape(perlinModule, xOffset, yOffset);

	//now let's create the mesh according to the new position
	createMesh();
	updateMesh();
}

void VoxelChunck::updateMesh() {
	//after adding all the data to the mesh, calculate the normals, tangents and send the data to the renderer
	this->cube->UpdateMeshData();
}


bool VoxelChunck::getIsVoxelActive(Vector3 position) const {
	return blocks[(int)position.x][(int)position.y][(int)position.z].getIsActive();
}