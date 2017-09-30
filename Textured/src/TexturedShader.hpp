#ifndef TEXTURED_SHADER_HPP
#define TEXTURED_SHADER_HPP

#include <dusk/Dusk.hpp>

class TexturedShader : public dusk::Shader
{
public:

    TexturedShader()
        : dusk::Shader({
            "assets/shaders/textured.vs.glsl",
            "assets/shaders/textured.fs.glsl",
        })
    { }

    void SetCameraPos(glm::vec3 pos)
    {
        _lightData.CameraPos = pos;
        updateLightData();
    }

    void SetLightPos(glm::vec3 pos)
    {
        _lightData.LightPos = pos;
        updateLightData();
    }

private:

    struct LightingData
    {
        alignas(4) glm::vec3 LightPos;
        alignas(4) glm::vec3 CameraPos;
    };

    void updateLightData()
    {
        Bind();
        SetUniformBufferData("LightingData", &_lightData);
    }

    LightingData _lightData;

};

#endif // TEXTURED_SHADER_HPP
