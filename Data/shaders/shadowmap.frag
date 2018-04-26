#version 330 core

in vec3 ex_UV;

out vec4 ColorOut;

uniform samplerCube uCubeMap;

void main()
{
	vec3 tex = ex_UV;
	//tex.x = -tex.x;
	ColorOut 		= vec4(texture(uCubeMap, tex).rgb, 1);
} 