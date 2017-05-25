#version 330 core

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out vec4 o_color;

void main()
{
    o_color = vec4(1, 0, 0, 1);
}
