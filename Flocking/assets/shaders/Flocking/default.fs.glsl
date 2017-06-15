#version 330 core

#include ../data/transform.inc.glsl
#include ../data/material.inc.glsl

in vec4 p_Position;
in vec4 p_Normal;
in vec2 p_TexCoord;

in vec3 p_LightDir;
in vec3 p_ViewDir;

out vec4 o_Color;

void main()
{
    vec3 ambient, diffuse, specular;
    vec4 normal;

    normal = normalize(p_Normal);
    if (_MaterialData.HasBumpMap)
    {
        normal = _TransformData.Model * (texture(_BumpMap, p_TexCoord).rgba * 2.0 - 1.0);
    }

    if (_MaterialData.HasAmbientMap)
    {
        ambient = texture(_AmbientMap, p_TexCoord).rgb;
    }
    else
    {
        ambient = _MaterialData.Ambient.rgb;
    }

    float diff = max(0.0, dot(normal.xyz, p_LightDir));
    diffuse    = diff * _MaterialData.Diffuse.rgb;

    if (_MaterialData.HasDiffuseMap)
    {
        diffuse = texture(_DiffuseMap, p_TexCoord).rgb;
    }

    vec3  halfwayDir = normalize(p_LightDir + p_ViewDir);
    float spec       = max(0.0, dot(normal.xyz, halfwayDir));
    if (_MaterialData.Shininess > 0)
    {
        spec = pow(spec, _MaterialData.Shininess);
    }

    if (_MaterialData.HasSpecularMap)
    {
        specular = spec * texture(_SpecularMap, p_TexCoord).rgb;
    }
    else
    {
        specular = vec3(spec);
    }

    //o_Color = vec4(ambient + diffuse + specular, 1.0);
    o_Color = vec4(_MaterialData.Ambient.rgb, 1.0);
}
