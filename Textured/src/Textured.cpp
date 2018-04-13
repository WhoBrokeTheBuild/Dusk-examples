#include <dusk/Main.hpp>
#include <dusk/Dusk.hpp>
#include <dusk/core/Event.hpp>
#include <sstream>

dusk::ScriptHost script;

dusk::Shader * shader;
dusk::Camera * camera;
dusk::Model * model;

std::unique_ptr<dusk::ICallback> updateCb, renderCb, reloadCb;

void Update(const dusk::UpdateContext& ctx)
{
    static float rotation = 0.0f;
    rotation += 0.5f * ctx.DeltaTime;
    rotation = fmod(rotation, 360.0f);
    glm::vec3 rot = model->GetRotation();
    rot.y = glm::radians(rotation);

    model->SetRotation(rot);
}

void Render(dusk::RenderContext& ctx)
{
    ctx.CurrentShader = shader;
    ctx.CurrentCamera = camera;
}

void _DuskSetup(dusk::App * app)
{
    script.LoadFile("assets/scripts/main.as");
    script.Run();

    shader = app->AddShader(std::unique_ptr<dusk::Shader>(new dusk::Shader({ "assets/shaders/textured.fs.glsl", "assets/shaders/textured.vs.glsl" })));

    updateCb.reset();
    updateCb = app->OnUpdate.AddStatic(&Update);

    renderCb.reset();
    renderCb = app->OnRender.AddStatic(&Render);

    dusk::Scene * scene = app->AddScene("main", std::make_unique<dusk::Scene>());
    app->SetActiveScene("main");

    camera = (dusk::Camera *)scene->AddChild(std::make_unique<dusk::Camera>(), "camera_main");
    camera->SetAspect(app->GetWindowSize());
    camera->SetPosition({ 3, 3, 3 });
    camera->SetForward({ -1, -1, -1 });

    dusk::Actor * tmp = scene->AddChild(std::make_unique<dusk::Actor>(), "actor_main");

    model = tmp->AddChild<dusk::Model>(std::make_unique<dusk::Model>(), "model_globe");
    model->AddMesh(std::make_shared<dusk::Mesh>("assets/models/globe/globe.obj"));
}
