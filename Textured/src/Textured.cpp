#include "Textured.hpp"

void _DuskSetup(dusk::App * app)
{
    app->AddScene("main_scene", std::make_unique<GameScene>());
    app->SetActiveScene("main_scene");
}
