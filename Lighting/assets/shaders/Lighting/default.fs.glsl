#version 330 core

in vec4 p_Position;
in vec4 p_Normal;
in vec2 p_TexCoord;

in vec3 p_LightDir;
in vec3 p_ViewDir;

out vec4 o_Color;

void main()
{
    vec3 color = vec3(0.102, 0.537, 0.133);

    vec3 ambient = 0.1 * color;

    float diff    = max(dot(p_Normal.xyz, p_LightDir), 0.0);
    vec3  diffuse = diff * color;

    vec3  halfway_dir = normalize(p_LightDir + p_ViewDir);
    float spec        = pow(max(dot(p_Normal.xyz, halfway_dir), 0.0), 16);
    vec3  specular    = spec * color;

    o_Color = vec4(ambient + diffuse + specular, 1.0);
}
