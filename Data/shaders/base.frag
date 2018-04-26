#version 330 core

// precision highp float;

in vec3 ex_Normal;
in vec2 ex_UV;

out vec4 WorldPosOut;

uniform sampler2D uDiffuseMap;
uniform sampler2D uNormalMap;


const int iGradientLevels = 4;

void main(void)
{
	//out_Color = vec4(texture( uDiffuseMap, ex_UV ).rgb, 1);
	WorldPosOut = vec4(ex_Normal.xyz /2 + 0.5, 1);
	//out_Color = vec4(ex_UV.xy, 0, 1);
	//out_Color = floor(out_Color * iGradientLevels) /iGradientLevels;
	//out_Color.a = 0.8;
}