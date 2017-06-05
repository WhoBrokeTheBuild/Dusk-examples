#version 330 core

layout(location = 0) in vec3 i_Position;
layout(location = 1) in vec3 i_Normal;
layout(location = 2) in vec2 i_TexCoord;

#include ../data/transform.inc.glsl

out vec3 p_Position;
out vec3 p_Normal;
out vec2 p_TexCoord;

void main()
{
    p_Position = i_Position;
    p_Normal   = i_Normal;
    p_TexCoord = i_TexCoord;

    gl_Position = _TransformData.MVP * vec4(p_Position, 1.0);
}
