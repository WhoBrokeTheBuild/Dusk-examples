#ifndef TEXTURED_HPP
#define TEXTURED_HPP

#include <dusk/Main.hpp>
#include <dusk/Dusk.hpp>
#include <dusk/core/Event.hpp>

#include <sstream>

class GameScene : public dusk::Scene
{
public:

    DUSK_SCENE(GameScene)

    GameScene(const std::string& id, const std::string filename = "")
        : dusk::Scene(id, filename)
    {
        dusk::App * app = dusk::App::Inst();
        TrackCallback(app->OnUpdate.AddMember<GameScene>(this, &GameScene::Update));
    }

    virtual void Start() override
    {
        Scene::Start();

        script.LoadFile("scripts/main.as");
        script.Run();
    }

    void Update(dusk::UpdateContext& ctx)
    {
        Scene::Update(ctx);

        rot += 0.01f * ctx.DeltaTime;

        const std::vector<dusk::Actor*> tmp = GetAllActors();
        if (!tmp.empty()) {
            tmp[0]->SetRotation({ 0.0f, rot, 0.0f });
        }
    }

private:

    float rot = 0.0f;

    dusk::ScriptHost script;

}; // class GameScene

#endif // TEXTURED_HPP
