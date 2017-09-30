#include <dusk/Main.hpp>
#include <dusk/Dusk.hpp>
#include "TexturedShader.hpp"
#include <sstream>

int _DuskMain(int argc, char** argv)
{
    dusk::App app(argc, argv);

    dusk::Scene * scene = app.AddScene(dusk::Scene::Create());

    dusk::Actor * actor = scene->AddActor(dusk::Actor::Create());
    actor->SetPosition({ 0, 0, 0 });

    dusk::ModelComponent * modelComp = actor->AddComponent<dusk::ModelComponent>(dusk::ModelComponent::Create(dusk::Model::Create()));
    dusk::Model * model = modelComp->GetModel();
    model->AddMesh(dusk::Mesh::Create("assets/models/globe/globe.obj"));

    dusk::Box bounds = model->GetBounds();

    dusk::Camera * camera = scene->AddCamera(dusk::Camera::Create());
    camera->SetAspect(app.GetWindowSize());
    camera->SetPosition(bounds.Max + (bounds.GetSize() * 0.5f));
    camera->SetForward(bounds.Min - bounds.Max);

    TexturedShader sp;
    sp.SetLightPos(camera->GetPosition());
    sp.SetCameraPos(camera->GetPosition());

    app.EvtUpdate.AddStatic([&](const dusk::UpdateContext& ctx) {
        glm::vec3 rot = model->GetRotation();
        rot.y += glm::radians(0.5f * ctx.DeltaTime);
        model->SetRotation(rot);

        model->Update(ctx);
    });

    app.EvtRender.AddStatic([&](dusk::RenderContext& ctx) {
        ctx.CurrentShader = &sp;
        ctx.CurrentCamera = camera;
        model->Render(ctx);
    });

    app.Start();
    return 0;
}
