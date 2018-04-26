#version 330 core

in vec3 in_Position;
in vec2 in_TexCoord;

out vec3 ex_UV;

uniform mat4 uMVP; // Projection * ModelView

void main(void)
{
	vec4 pos = uMVP * vec4(in_Position, 1.0);
    gl_Position = pos.xyww; // no Z to have a depth of one !
	ex_UV = in_Position;
}