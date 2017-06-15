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
LightingData lighting_data;

void AppUpdate(const dusk::Event& event)
{
    dusk::App * app = dusk::App::GetInst();

    glm::vec3 cam_pos = app->GetScene()->GetCamera()->GetPosition();
    lighting_data.LightPos  = cam_pos;
    lighting_data.CameraPos = cam_pos;
    dusk::Shader::UpdateData("LightingData", &lighting_data, sizeof(lighting_data));
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
            ss.clear();
            ss.str("boid-");
            ss << g << "-" << i;

            dusk::Actor * actor = (dusk::Actor *)new BoidActor(scene, ss.str(), g, BOID_COLORS[g]);
            app->GetScene()->AddActor(actor);
        }
    }
}

int main(int argc, char** argv)
{
    dusk::App app(argc, argv);

    dusk::Shader::DefineData("LightingData", sizeof(LightingData));
    //app.RegisterActorType<BoidActor>("Boid");

	app.LoadConfig("assets/config/Flocking/app.json");
    app.AddEventListener((dusk::EventID)dusk::App::Events::UPDATE, &AppUpdate);
    app.AddEventListener((dusk::EventID)dusk::App::Events::START, &AppStart);

    app.Run();
	return 0;
}
