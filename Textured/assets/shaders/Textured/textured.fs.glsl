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
    vec4 normal = normalize(p_Normal);
    if ((_MaterialData.MapFlags & BumpMapFlag) > 0u)
    {
        normal = _TransformData.Model * (texture(_BumpMap, p_TexCoord).rgba * 2.0 - 1.0);
    }

    vec3 ambient = _MaterialData.Ambient.rgb;
    if ((_MaterialData.MapFlags & AmbientMapFlag) > 0u)
    {
        ambient = texture(_AmbientMap, p_TexCoord).rgb;
    }

    float diff = max(0.0, dot(normal.xyz, p_LightDir));
    vec3 diffuse = diff * _MaterialData.Diffuse.rgb;

    if ((_MaterialData.MapFlags & DiffuseMapFlag) > 0u)
    {
        diffuse = diff * texture(_DiffuseMap, p_TexCoord).rgb;
    }

    vec3  halfwayDir = normalize(p_LightDir + p_ViewDir);
    float spec       = max(0.0, dot(normal.xyz, halfwayDir));
    if (_MaterialData.Shininess > 0)
    {
        spec = pow(spec, _MaterialData.Shininess);
    }

    vec3 specular = vec3(spec);
    if ((_MaterialData.MapFlags & SpecularMapFlag) > 0u)
    {
        specular = spec * texture(_SpecularMap, p_TexCoord).rgb;
    }

    o_Color = vec4(ambient + diffuse + specular, 1.0);
}
