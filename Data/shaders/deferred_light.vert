#version 420

in vec3 in_Position;
in vec2 in_TexCoord;

uniform mat4 uLightWVP;

out vec2 ex_TexCoord;
out vec4 ex_LightSpacePos;

void main()
{
    gl_Position = vec4(in_Position, 1.0);
    ex_TexCoord = in_TexCoord;
    ex_LightSpacePos = uLightWVP * vec4(in_Position, 1);
}