#version 330 core

in vec3 in_Position;

uniform mat4 uMVP; // Projection * ModelView

void main(void)
{
    gl_Position = uMVP * vec4(in_Position, 1.0);
}