#include <dusk/Main.hpp>
#include <dusk/Dusk.hpp>
#include "TexturedShader.hpp"
#include <sstream>

dusk::Camera * camera;
dusk::Model * model;
TexturedShader * sp;

void Update(const dusk::UpdateContext& ctx)
{
    static float rotation = 0.0f;
    rotation += 0.5f * ctx.DeltaTime;
    rotation = fmod(rotation, 360.0f);

    glm::vec3 rot = model->GetRotation();
    rot.y = glm::radians(rotation);
    model->SetRotation(rot);

    model->Update(ctx);
}

void Render(dusk::RenderContext& ctx)
{
    ctx.CurrentShader = sp;
    ctx.CurrentCamera = camera;
    model->Render(ctx);
}

void _DuskSetup(dusk::App * app)
{
    dusk::Scene * scene = app->AddScene(dusk::Scene::Create());

    dusk::Actor * actor = scene->AddActor(dusk::Actor::Create());
    actor->SetPosition({ 0, 0, 0 });

    dusk::ModelComponent * modelComp = actor->AddComponent<dusk::ModelComponent>(dusk::ModelComponent::Create(dusk::Model::Create()));
    model = modelComp->GetModel();
    model->AddMesh(dusk::Mesh::Create("assets/models/globe/globe.obj"));
    //model->AddMesh(dusk::Mesh::Create("/home/stephen/Downloads/dota2/models/heroes/meepo/fbx/meepo_arms.fbx"));
    //model->AddMesh(dusk::Mesh::Create("/home/stephen/Downloads/dota2/models/heroes/meepo/fbx/meepo_back.fbx"));
    //model->AddMesh(dusk::Mesh::Create("/home/stephen/Downloads/dota2/models/heroes/meepo/fbx/meepo_body_tex.fbx"));
    //model->AddMesh(dusk::Mesh::Create("/home/stephen/Downloads/dota2/models/heroes/meepo/fbx/meepo_head.fbx"));
    //model->AddMesh(dusk::Mesh::Create("/home/stephen/Downloads/dota2/models/heroes/meepo/fbx/meepo_shoulders.fbx"));
    //model->AddMesh(dusk::Mesh::Create("/home/stephen/Downloads/dota2/models/heroes/meepo/fbx/meepo_tail.fbx"));
    //model->AddMesh(dusk::Mesh::Create("/home/stephen/Downloads/dota2/models/heroes/meepo/fbx/meepo_weapon.fbx"));

    dusk::Box bounds = model->GetBounds();

    camera = scene->AddCamera(dusk::Camera::Create());
    camera->SetAspect(app->GetWindowSize());
    camera->SetPosition(bounds.Max + (bounds.GetSize() * 0.5f));
    camera->SetForward(bounds.Min - bounds.Max);

    app->EvtWindowResize.AddStatic([&](glm::ivec2 size) {
        camera->SetAspect(size);
    });

    sp = static_cast<TexturedShader *>(app->AddShader(std::make_unique<TexturedShader>()));
    sp->SetLightPos(camera->GetPosition());
    sp->SetCameraPos(camera->GetPosition());

    app->EvtUpdate.AddStatic(&Update);
    app->EvtRender.AddStatic(&Render);

}
