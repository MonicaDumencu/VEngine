#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D sceneTex;
uniform sampler2D reflectOffset;

in Vertex	{
	vec2 texCoord;
} IN;

out vec4 gl_FragColor;

void main(void)	{
	vec2 offset   = texture(reflectOffset, IN.texCoord).xy;
	gl_FragColor = texture(sceneTex, IN.texCoord + offset*0.02)*0.5 + texture(diffuseTex, IN.texCoord);
}