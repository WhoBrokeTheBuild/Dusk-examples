#include <dusk/Dusk.hpp>

struct LightingData
{
    alignas(4) glm::vec3 LightPos;
    alignas(4) glm::vec3 CameraPos;
};
LightingData _LightingData;

void Update(const dusk::Event& event)
{
    dusk::App * app = dusk::App::GetInst();

    glm::vec3 camPos = app->GetScene()->GetCurrentCamera()->GetPosition();

    _LightingData.LightPos  = camPos;
    _LightingData.CameraPos = camPos;
    dusk::Shader::UpdateData("LightingData", &_LightingData, sizeof(_LightingData));
}

int main(int argc, char** argv)
{
    dusk::App app(argc, argv);

    dusk::Shader::AddData("LightingData", &_LightingData, sizeof(_LightingData));

    app.LoadConfig("assets/config/Textured/app.json");
    app.AddEventListener((dusk::EventID)dusk::App::Events::UPDATE, &Update);

    app.Run();
    return 0;
}
