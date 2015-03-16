#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

uniform vec4 nodeColour;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out Vertex
{
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
	vec2 reflectOffset;
}OUT;

void main(void)
{
	OUT.colour = nodeColour;
	OUT.texCoord = (textureMatrix * vec4(texCoord,0.0,1.0)).xy;
	
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));

	vec3 waveNormal = normalize(normalMatrix * normalize(normal));

	OUT.normal = waveNormal;
	OUT.worldPos = (modelMatrix * vec4(position,1)).xyz;
	gl_Position = (projMatrix * viewMatrix * modelMatrix) * vec4(position, 1.0);

	vec3 flatNormal = waveNormal - dot(waveNormal, vec3(0, 1, 0)) * vec3(0, 1, 0);
	vec3 eyeNormal  = normalMatrix * flatNormal;
	OUT.reflectOffset = normalize(eyeNormal.xz) * length(flatNormal);
}