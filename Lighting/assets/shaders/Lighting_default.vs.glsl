#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;

#include data/lighting.inc.glsl
#include data/transform.inc.glsl

out vec4 position;
out vec4 normal;
out vec2 texcoord;

out vec3 light_dir;
out vec3 view_dir;

void main()
{
    position = transform_data.model * vec4(in_position, 1.0);
    normal   = transform_data.model * vec4(in_normal, 1.0);
    texcoord = in_texcoord;

    vec3 position_worldspace = vec3(transform_data.model * position);

    light_dir = lighting_data.light_pos - position_worldspace;
    light_dir = normalize(light_dir);

    view_dir = lighting_data.camera_pos - position_worldspace;
    view_dir = normalize(view_dir);

    gl_Position = transform_data.mvp * vec4(in_position, 1.0);
}
