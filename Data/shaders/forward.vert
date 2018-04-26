#version 330 core

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

out vec3 ex_Normal;
out vec2 ex_UV;

uniform mat4 uMVP; // Projection * ModelView

void main(void)
{
    gl_Position = uMVP * vec4(in_Position, 1.0);
	ex_Normal = in_Normal;
	ex_UV = in_TexCoord.xy;
}