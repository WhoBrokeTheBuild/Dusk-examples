#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;

#include data/lighting.inc.glsl
#include data/transform.inc.glsl
#include data/material.inc.glsl

out vec4 p_position;
out vec4 p_normal;
out vec2 p_texcoord;

out vec3 light_dir;
out vec3 view_dir;

void main()
{
    p_texcoord = vec2(in_texcoord.x, in_texcoord.y);
    p_position = transform_data.model * vec4(in_position, 1.0);
    p_normal   = transform_data.model * vec4(in_normal, 1.0);

    light_dir = lighting_data.light_pos - p_position.xyz;
    light_dir = normalize(light_dir);

    view_dir = lighting_data.camera_pos - p_position.xyz;
    view_dir = normalize(view_dir);

    gl_Position = transform_data.mvp * vec4(in_position, 1.0);
}
