#include <dusk/Dusk.hpp>

struct LightingData
{
    alignas(4) glm::vec3 LightPos;
    alignas(4) glm::vec3 CameraPos;
};
LightingData _LightingData;

void OnUpdate(const dusk::Event& event)
{
    dusk::App * app = dusk::App::GetInst();

    glm::vec3 camPos = app->GetScene()->GetCurrentCamera()->GetPosition();

    _LightingData.LightPos  = camPos;
    _LightingData.CameraPos = camPos;
    dusk::Shader::UpdateData("LightingData", &_LightingData, sizeof(_LightingData));
}

void OnStart(const dusk::Event& event)
{
    dusk::App * app = dusk::App::GetInst();
    dusk::Scene * scene = app->GetScene();

    dusk::Actor * redPiece = scene->GetActorTemplate("red-piece");
    dusk::Actor * blackPiece = scene->GetActorTemplate("black-piece");

    if (!redPiece || !blackPiece)
    {
        DuskLogError("Missing red-piece or black-piece");
        return;
    }

    std::vector<glm::vec3> redStart = {
        { -45.0f, 0.0f,  35.0f },
        { -45.0f, 0.0f,  15.0f },
        { -45.0f, 0.0f,  -5.0f },
        { -45.0f, 0.0f, -25.0f },
        { -45.0f, 0.0f, -45.0f },

        { -35.0f, 0.0f,  45.0f },
        { -35.0f, 0.0f,  25.0f },
        { -35.0f, 0.0f,   5.0f },
        { -35.0f, 0.0f, -15.0f },
        { -35.0f, 0.0f, -35.0f },

        { -25.0f, 0.0f,  35.0f },
        { -25.0f, 0.0f,  15.0f },
        { -25.0f, 0.0f,  -5.0f },
        { -25.0f, 0.0f, -25.0f },
        { -25.0f, 0.0f, -45.0f },
    };

    std::vector<glm::vec3> blackStart = {
        { 45.0f, 0.0f,  45.0f },
        { 45.0f, 0.0f,  25.0f },
        { 45.0f, 0.0f,   5.0f },
        { 45.0f, 0.0f, -15.0f },
        { 45.0f, 0.0f, -35.0f },

        { 35.0f, 0.0f,  35.0f },
        { 35.0f, 0.0f,  15.0f },
        { 35.0f, 0.0f,  -5.0f },
        { 35.0f, 0.0f, -25.0f },
        { 35.0f, 0.0f, -45.0f },

        { 25.0f, 0.0f,  45.0f },
        { 25.0f, 0.0f,  25.0f },
        { 25.0f, 0.0f,   5.0f },
        { 25.0f, 0.0f, -15.0f },
        { 25.0f, 0.0f, -35.0f },
    };

    for (glm::vec3& pos : redStart)
    {
        std::unique_ptr<dusk::Actor> newPiece = redPiece->Clone();
        newPiece->SetPosition(pos);
        scene->AddActor(std::move(newPiece));
    }

    for (glm::vec3& pos : blackStart)
    {
        std::unique_ptr<dusk::Actor> newPiece = blackPiece->Clone();
        newPiece->SetPosition(pos);
        scene->AddActor(std::move(newPiece));
    }
}

int main(int argc, char** argv)
{
    dusk::App app(argc, argv);

    dusk::Shader::AddData("LightingData", &_LightingData, sizeof(_LightingData));

    app.LoadConfig("assets/config/Checkers/app.json");
    app.AddEventListener((dusk::EventID)dusk::App::Events::UPDATE, &OnUpdate);
    app.AddEventListener((dusk::EventID)dusk::App::Events::START, &OnStart);

    app.Run();
    return 0;
}
