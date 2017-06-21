#include <dusk/Dusk.hpp>
#include <sstream>

#include "Constants.hpp"
#include "BoidActor.hpp"
#include "Octant.hpp"

const unsigned int BOID_COUNT = 500;

struct LightingData
{
    alignas(4) glm::vec3 LightPos;
    alignas(4) glm::vec3 CameraPos;
};
LightingData _LightingData;

void AppUpdate(const dusk::Event& event)
{
    dusk::App * app = dusk::App::GetInst();

    glm::vec3 camPos = app->GetScene()->GetCurrentCamera()->GetPosition();

    _LightingData.LightPos  = camPos;
    _LightingData.CameraPos = camPos;
    dusk::Shader::UpdateData("LightingData", &_LightingData, sizeof(_LightingData));
}

void AppStart(const dusk::Event& event)
{
    dusk::App * app = dusk::App::GetInst();

    std::stringstream ss;
    dusk::Scene * scene = app->GetScene();
    for (unsigned int g = 0; g < BOID_GROUPS; ++g)
    {
        for (unsigned int i = 0; i < BOID_COUNT; ++i)
        {
            scene->AddActor(std::unique_ptr<dusk::Actor>(new BoidActor(scene, g, BOID_COLORS[g])));
        }
    }
}

int main(int argc, char** argv)
{
    dusk::App app(argc, argv);

    dusk::Shader::AddData("LightingData", &_LightingData, sizeof(_LightingData));

	app.LoadConfig("assets/config/Flocking/app.json");
    app.AddEventListener((dusk::EventID)dusk::App::Events::UPDATE, &AppUpdate);
    app.AddEventListener((dusk::EventID)dusk::App::Events::START, &AppStart);

    app.Run();
	return 0;
}
