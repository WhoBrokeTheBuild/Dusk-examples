#version 330 core

#include data/material.inc.glsl
#include data/transform.inc.glsl

in vec4 p_position;
in vec4 p_normal;
in vec2 p_texcoord;

in vec3 light_dir;
in vec3 view_dir;

out vec4 o_color;

void main()
{
    vec3 ambient, diffuse, specular;
    vec4 normal;

    normal = normalize(p_normal);
    if (material_data.has_bump_map)
    {
        normal = transform_data.model * (texture(bump_map, p_texcoord).rgba * 2.0 - 1.0);
    }

    if (material_data.has_ambient_map)
    {
        ambient = texture(ambient_map, p_texcoord).rgb;
    }
    else
    {
        ambient = material_data.ambient.rgb;
    }

    float diff = max(0.0, dot(normal.xyz, light_dir));
    diffuse    = diff * material_data.diffuse.rgb;

    if (material_data.has_diffuse_map)
    {
        diffuse = texture(diffuse_map, p_texcoord).rgb;
    }

    vec3  halfway_dir = normalize(light_dir + view_dir);
    float spec        = max(0.0, dot(normal.xyz, halfway_dir));
    if (material_data.shininess > 0)
    {
        spec = pow(spec, material_data.shininess);
    }

    if (material_data.has_specular_map)
    {
        specular = spec * texture(specular_map, p_texcoord).rgb;
    }
    else
    {
        specular = vec3(spec);
    }

    o_color = vec4(ambient + diffuse + specular, 1.0);
}
