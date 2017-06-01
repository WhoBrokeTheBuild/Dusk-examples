#include <dusk/Dusk.hpp>

struct LightingData
{
    alignas(4) glm::vec3 light_pos;
    alignas(4) glm::vec3 camera_pos;
};
LightingData lighting_data;

void Update(const dusk::Event& event)
{
    dusk::App * app = dusk::App::GetInst();

    glm::vec3 cam_pos = app->GetScene()->GetCamera()->GetPosition();

    lighting_data.light_pos  = cam_pos;
    lighting_data.camera_pos = cam_pos;
    dusk::Shader::UpdateData("LightingData", &lighting_data, sizeof(lighting_data));
}

int main(int argc, char** argv)
{
    dusk::App app(argc, argv);

    dusk::Shader::DefineData("LightingData", sizeof(LightingData));

    app.LoadConfig("assets/config/Textured_example.json");
    app.AddEventListener((dusk::EventID)dusk::App::Events::UPDATE, &Update);

    app.Run();
    return 0;
}
