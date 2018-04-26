#version 330 core

in vec3 ex_Normal;
in vec2 ex_UV;

out vec4 ColorOut;

uniform sampler2D uDiffuseMap;

void main()
{
	ColorOut 		= vec4(texture(uDiffuseMap, ex_UV).rgb, 1);
	//ColorOut		= vec4(ex_UV, texture(uDiffuseMap, ex_UV).r, 1);
} 