#ifndef TEXTURED_HPP
#define TEXTURED_HPP

#include <dusk/Main.hpp>
#include <dusk/Dusk.hpp>
#include <dusk/core/Event.hpp>

#include <sstream>

class GameScene : public dusk::Scene
{
public:

    DUSK_CLASS(GameScene)

    GameScene(std::string id)
        : dusk::Scene(id)
    { }

    virtual void Start() override
    {
        Scene::Start();

        script.LoadFile("scripts/main.as");
        script.Run();

        dusk::App * app = dusk::App::Inst();

        dusk::Shader * shader = app->AddShader(std::make_unique<dusk::Shader>(std::vector<std::string>({
            "shaders/textured.fs.glsl",
            "shaders/textured.vs.glsl"
        })));

        TrackCallback(app->OnUpdate.AddMember<GameScene>(this, &GameScene::Update));

        dusk::Camera * camera = AddActor<dusk::Camera>(std::make_unique<dusk::Camera>("main_camera", this));
        camera->SetAspect(app->GetWindowSize());
        camera->SetPosition({ 3, 3, 3 });
        camera->SetForward({ -1, -1, -1 });

        actor = AddActor(std::make_unique<dusk::Actor>("main_actor", this));
        actor->AddComponent(std::make_unique<dusk::MeshComponent>(actor, std::make_unique<dusk::Mesh>("models/globe/globe.obj")));

        app->GetRenderContext().CurrentShader = shader;
        app->GetRenderContext().CurrentCamera = camera;
    }

    void Update(const dusk::UpdateContext& ctx)
    {
        glm::vec3 rot = actor->GetRotation();
        rot.y += glm::radians(0.5f * ctx.DeltaTime);
        rot.y = fmod(rot.y, M_PI * 2.0f);
        actor->SetRotation(rot);
    }

private:

    dusk::ScriptHost script;

    dusk::Actor * actor;

}; // class GameScene

#endif // TEXTURED_HPP
