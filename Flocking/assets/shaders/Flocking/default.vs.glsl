#version 330 core

layout(location = 0) in vec3 i_Position;
layout(location = 1) in vec3 i_Normal;
layout(location = 2) in vec2 i_TexCoord;

layout(std140) uniform LightingData
{
    vec3 LightPos;
    vec3 CameraPos;

} _LightingData;

#include ../data/transform.inc.glsl
#include ../data/material.inc.glsl

out vec4 p_Position;
out vec4 p_Normal;
out vec2 p_TexCoord;

out vec3 p_LightDir;
out vec3 p_ViewDir;

void main()
{
    p_Position = _TransformData.Model * vec4(i_Position, 1.0);
    p_Normal   = _TransformData.Model * vec4(i_Normal, 1.0);
    p_TexCoord = i_TexCoord;

    p_LightDir = _LightingData.LightPos - p_Position.xyz;
    p_LightDir = normalize(p_LightDir);

    p_ViewDir = _LightingData.CameraPos - p_Position.xyz;
    p_ViewDir = normalize(p_ViewDir);

    gl_Position = _TransformData.MVP * vec4(i_Position, 1.0);
}
