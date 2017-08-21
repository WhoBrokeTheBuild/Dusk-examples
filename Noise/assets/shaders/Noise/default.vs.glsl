#version 330 core

in vec3 _Position;
in vec3 _Normal;
in vec2 _TexCoord;

#include ../data/transform.inc.glsl

out vec4 p_Position;
out vec4 p_Normal;
out vec2 p_TexCoord;

void main()
{
    p_Position = _TransformData.Model * vec4(_Position, 1.0);
    p_Normal   = _TransformData.Model * vec4(_Normal, 1.0);
    p_TexCoord = _TexCoord;
    
    gl_Position = _TransformData.MVP * vec4(_Position, 1.0);
}
