#version 330 core

in vec3 ex_WorldPos;
in vec3 ex_Normal;
in vec2 ex_UV;

out vec3 TexCoordOut;
out vec3 WorldPosOut;
out vec3 AlbedoOut;
out vec3 NormalOut;
out float ObjectIdOut;

uniform sampler2D uDiffuseMap;
uniform uint uObjectId;

void main()
{
	WorldPosOut 	= ex_WorldPos;
	AlbedoOut 		= texture(uDiffuseMap, ex_UV).rgb;
	NormalOut 		= normalize(ex_Normal);
	TexCoordOut 	= vec3(ex_UV, 0);
	float fObjectId = uObjectId;
	ObjectIdOut		= uObjectId;
} 