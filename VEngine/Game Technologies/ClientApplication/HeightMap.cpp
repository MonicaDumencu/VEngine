#include "HeightMap.h"

HeightMap::HeightMap() {
	//createHeightMapForLandscape();
	/*grassTexture = SOIL_load_OGL_texture(TEXTUREDIR"grass.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	grassBumpTexture = SOIL_load_OGL_texture(TEXTUREDIR"GRASS_NORMAL.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	sandTexture = SOIL_load_OGL_texture(TEXTUREDIR"sand.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	sandBumpTexture = SOIL_load_OGL_texture(TEXTUREDIR"sand.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	mudTexture = SOIL_load_OGL_texture(TEXTUREDIR"mud.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	mudBumpTexture = SOIL_load_OGL_texture(TEXTUREDIR"mud.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	rockTexture = SOIL_load_OGL_texture(TEXTUREDIR"rock.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	rockBumpTexture = SOIL_load_OGL_texture(TEXTUREDIR"rock.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);*/
}

HeightMap::~HeightMap() {

}

void HeightMap::setHeightAndTextureFromHeightMap(int x, float &y, int z, GLuint texture, GLuint bumpTexture) {
	float nrX = (x + xOffsetForNextChunck) * 0.01f;
	float nrZ = (z + zOffsetForNextChunck) * 0.01f;

	y = (perlinModule.GetValue(nrX, 0, nrZ)) * 10;

	/*if (y < 3.0f) {
		texture = grassTexture;
		bumpTexture = bumpTexture;
		return;
	}
	if (y < 5.0f) {
		texture = sandTexture;
		bumpTexture = sandBumpTexture;
		return;
	}
	texture = rockTexture;
	bumpTexture = rockBumpTexture;*/
}