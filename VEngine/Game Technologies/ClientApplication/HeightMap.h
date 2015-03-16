/*
Author: Monica
Purpose: Creates the noise for the heightmap and manages the textures for the different kind of terrain
Modified: 
*/
#pragma once

#include "../../libnoise/include/noise/noise.h"
#include "noiseutils.h"
#include "../../nclgl/OGLRenderer.h"

class HeightMap {
public:
	HeightMap();
	~HeightMap();

	void setHeightAndTextureFromHeightMap(int x, float &y, int z, GLuint texture, GLuint bumpTexture);

	float getXOffsetForNextChunck() const { return xOffsetForNextChunck; }
	float getZOffsetForNextChunck() const { return zOffsetForNextChunck; }

	void setXOffsetForNextChunck(float val) { xOffsetForNextChunck = val; }
	void setZOffsetForNextChunck(float val) { zOffsetForNextChunck = val; }
private:
	module::Perlin perlinModule;

	GLuint atlasTexture;
	GLuint atlasBumpTexture;

	float xOffsetForNextChunck;
	float zOffsetForNextChunck;
};
