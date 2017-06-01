#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;

#include transform_data.inc.glsl

out vec3 position;
out vec3 normal;
out vec2 texcoord;

void main()
{
    position = in_position;
    normal   = in_normal;
    texcoord = in_texcoord;

    gl_Position = transform_data.mvp * vec4(position, 1.0);
}
