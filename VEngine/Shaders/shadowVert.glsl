#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform int isInversed;

in vec3 position;

void main(void)
{
	vec4 worldPos = modelMatrix * vec4(position, 1.0);
	if(isInversed==1)
	{
		worldPos.y = -1000 - worldPos.y;
	}
	gl_Position = projMatrix * viewMatrix * worldPos;
}