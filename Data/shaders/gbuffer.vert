#version 330 core

in vec3 in_Position;
in vec2 in_TexCoord;

out vec2 ex_UV;

void main(void)
{
    gl_Position = vec4(in_Position, 1.0);
	ex_UV = in_TexCoord.xy;
}