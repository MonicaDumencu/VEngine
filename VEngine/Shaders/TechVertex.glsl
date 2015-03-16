#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;
uniform int  isInversed;

uniform vec4 nodeColour;

in  vec3 position;
in  vec2 texCoord;

out Vertex	{
	vec2 texCoord;
	vec4 colour;
} OUT;

void main(void)	{
	vec4 worldPos = modelMatrix * vec4(position,1.f);
	if(isInversed == 1)
	{
		worldPos.y = -1000 - worldPos.y;
	}
	gl_Position		=  projMatrix * viewMatrix * worldPos;
	OUT.texCoord	= (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;
	OUT.colour		= nodeColour;
}