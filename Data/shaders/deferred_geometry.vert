#version 330 core

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

out vec3 ex_WorldPos;
out vec3 ex_Normal;
out vec2 ex_UV;

uniform mat4 uModelMatrix;
uniform mat4 uMVPMatrix; // Projection * View * Model
uniform mat4 uNormalMtx;

void main(void)
{
    gl_Position = uMVPMatrix * vec4(in_Position, 1.0);			// valid
    ex_WorldPos = (uModelMatrix * vec4(in_Position, 1.0)).xyz;	// valid
	ex_Normal = (uNormalMtx * vec4(in_Normal, 1.0)).xyz;
	ex_UV = in_TexCoord.xy;										// valid ?
}