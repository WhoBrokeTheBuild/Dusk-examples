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

        dusk::Shader * shader = app->AddShader(std::make_unique<dusk::Shader>("main_shader", std::vector<std::string>({
            "shaders/textured.fs.glsl",
            "shaders/textured.vs.glsl"
        })));

        TrackCallback(app->OnUpdate.AddMember<GameScene>(this, &GameScene::Update));

        dusk::Camera * camera = AddCamera(std::make_unique<dusk::Camera>("main_camera", this));
        camera->SetAspect(app->GetWindowSize());
        camera->SetPosition({ 0, 0, 3 });
        camera->SetLookAt({ 0, 0, 0 });

        actor = AddActor(std::make_unique<dusk::Actor>("main_actor", this));

        dusk::Mesh * mesh = dusk::AssetLoader::Load<dusk::Mesh>("models/sponza/sponza.obj");
        actor->AddComponent(std::make_unique<dusk::MeshComponent>(actor, mesh));

        app->GetRenderContext().CurrentShader = shader;
        app->GetRenderContext().CurrentCamera = camera;
    }

    void Update(const dusk::UpdateContext& ctx)
    {
        glm::vec3 rot = actor->GetRotation();
        rot.y -= glm::radians(0.5f * ctx.DeltaTime);
        rot.y = fmod(rot.y, M_PI * 2.0f);
        actor->SetRotation(rot);
    }

private:

    dusk::ScriptHost script;

    dusk::Actor * actor;

}; // class GameScene

#endif // TEXTURED_HPP
