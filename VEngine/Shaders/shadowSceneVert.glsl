#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;
uniform mat4 shadowMatrix;
uniform int isInversed;

uniform vec4 nodeColour;

in vec3 position;
in vec3 normal;
in vec3 tangent;
in vec2 texCoord;

out Vertex
{
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 worldPos;
	vec4 shadowProj;
} OUT;

void main(void)
{
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	
	OUT.colour = nodeColour;
	OUT.texCoord = texCoord;
	
	OUT.normal = normalize(normalMatrix * normalize(normal));
	OUT.tangent = normalize(normalMatrix * normalize(tangent));
	OUT.binormal = normalize(normalMatrix * normalize(cross(normal,tangent)));
	vec4 worldPos = modelMatrix * vec4(position,1);
	if(isInversed==1)
	{
		worldPos.y = - 1000 - worldPos.y;
	}
	OUT.worldPos = worldPos.xyz;
	OUT.shadowProj = (shadowMatrix * vec4(position + (normal * 0.1),1));
	
	gl_Position = projMatrix * viewMatrix * worldPos;
}