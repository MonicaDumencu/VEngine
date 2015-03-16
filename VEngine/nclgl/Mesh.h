/******************************************************************************
Class:Mesh
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:Wrapper around OpenGL primitives, geometry and related 
OGL functions.

There's a couple of extra functions in here that you didn't get in the tutorial
series, to draw debug normals and tangents. 


-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "OGLRenderer.h"
#include <vector>

//A handy enumerator, to determine which member of the bufferObject array
//holds which data
enum MeshBuffer {
	VERTEX_BUFFER	,
	COLOUR_BUFFER	, 
	TEXTURE_BUFFER	,
	NORMAL_BUFFER	, 
	BINORMAL_BUFFER ,
	TANGENT_BUFFER	,
	INDEX_BUFFER	,
	MAX_BUFFER
};

class Mesh	{
public:
	friend class MD5Mesh;
	Mesh(void);
	virtual ~Mesh(void);

	virtual void Draw(bool update = true);

	//Generates a single triangle, with RGB colours
	static Mesh*	GenerateTriangle();
	

	//Generates a single white quad, going from -1 to 1 on the x and z axis.
	static Mesh*	GenerateQuad();
	static Mesh*	GenerateQuadAlt();
	//Generates a coloured quad, going from -1 to 1 on the x and z axis, with adjustable texture coords.
	static Mesh*	GenerateQuadTexCoordCol(Vector2 scale, Vector2 texCoord, Vector4 colour); //NX 01/11/2012

	//Sets the Mesh's diffuse map. Takes an OpenGL texture 'name'
	void	SetTexture(GLuint tex)	{
		texture = tex;
		if (texture != 0)
			glGenerateMipmap(texture);
	}
	//Gets the Mesh's diffuse map. Returns an OpenGL texture 'name'
	GLuint  GetTexture()			{return texture;}

	void	SetColour(Vector4* colour) {colours = colour;} //NX 24/10/2012

	//Sets the Mesh's bump map. Takes an OpenGL texture 'name'
	void	SetBumpMap(GLuint tex)	{
		bumpTexture = tex;
		if (texture != 0)
			glGenerateMipmap(bumpTexture);
	}
	//Gets the Mesh's bump map. Returns an OpenGL texture 'name'
	GLuint  GetBumpMap()			{return bumpTexture;}

	//Extra stuff!!!! Aren't I nice?
	void	DrawDebugNormals(float length = 5.0f);
	void	DrawDebugTangents(float length = 5.0f);

	bool	TransformsTexCoords() { return transformCoords;}

	void SetTextureAndBumpmapRepeat(const bool& isRepeating);

	// --------------- Added by Monica ----------------------- ////
	//Method for adding a cube's data to the mesh.
	void	AddCubeToMesh(Vector3* arrayVertices, Vector2* arrayTextures, Vector3* arrayNormals, Vector3* arrayTangents, int nrVert);
	//update the data in the mesh buffers
	void	UpdateMeshData();
	//get a pointer to the mesh data from in buffers
	void*	GetMapBuffer(MeshBuffer buffOb, float size);
	//bind the buffers dynamically
	void	BindBuffers(const int nrVert);
	//reset the number of vertices to 0
	void	DeleteOldData();

	//generate a cube for the terrain
	static Mesh*	GenerateCube();
	// ------------------------------------------------------- ///
	//Helper function for GenerateTangents
	static Vector3 GenerateTangent(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector2 &ta, const Vector2 &tb, const Vector2 &tc);

protected:
	//Buffers all VBO data into graphics memory. Required before drawing!
	void	BufferData();


	//Generates normals for all facets. Assumes geometry type is GL_TRIANGLES...
	void	GenerateNormals();

	//Generates tangents for all facets. Assumes geometry type is GL_TRIANGLES...
	void	GenerateTangents();
	
	//VAO for this mesh
	GLuint	arrayObject;
	//VBOs for this mesh
	GLuint	bufferObject[MAX_BUFFER];
	//Number of vertices for this mesh
	GLuint	numVertices;
	//Primitive type for this mesh (GL_TRIANGLES...etc)
	GLuint	type;
	//OpenGL texture name for the diffuse map
	GLuint	texture;

	//Stuff introduced later on in the tutorials!!

	//Number of indices for this mesh
	GLuint			numIndices;

	//OpenGL texture name for the bump map
	GLuint			bumpTexture;

	//You might wonder why we keep pointers to vertex data once
	//it's sent off to graphics memory. For basic meshes, there's no
	//real need to keep it, so feel free to delete it. For MD5Meshes,
	//we need access to the vertex data for skinning per frame...

	//Pointer to vertex position attribute data (badly named...?)
	Vector3*		vertices;
	//Pointer to vertex colour attribute data
	Vector4*		colours;
	//Pointer to vertex texture coordinate attribute data
	Vector2*		textureCoords;
	//Pointer to vertex normals attribute data
	Vector3*		normals;
	Vector3*        binormals;
	//Pointer to vertex tangents attribute data
	Vector3*		tangents;
	//Pointer to vertex indices attribute data
	unsigned int*	indices;


	bool			transformCoords;

	// ---------- Added by Monica -------------- //
	//unmap after updating the data in the buffers
	void	UnmapBufferData();
	void	GenerateNormalsTerrain();
	void	GenerateTangentsTerrain();

	Vector3* ptrVertex;
	Vector2* ptrTexCoord;
	Vector3* ptrNormals;
	Vector3* ptrTangents;
	// ----------------------------------------- //

	bool memoryMark;		//for memory foot
};

