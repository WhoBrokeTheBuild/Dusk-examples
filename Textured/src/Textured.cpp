#include "Textured.hpp"

void _DuskSetup(dusk::App * app)
{
    app->SetActiveScene(app->AddScene(std::make_unique<GameScene>("main_scene")));
}
