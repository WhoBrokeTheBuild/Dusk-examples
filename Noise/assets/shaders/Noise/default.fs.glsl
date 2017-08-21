#version 330 core

in vec4 p_Position;
in vec4 p_Normal;
in vec2 p_TexCoord;

out vec4 o_Color;

uniform sampler2D u_Noise;

void main()
{
    o_Color = texture(u_Noise, p_TexCoord);
}
