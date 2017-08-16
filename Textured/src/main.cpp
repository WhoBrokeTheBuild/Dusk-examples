#include <dusk/Dusk.hpp>

struct LightingData
{
    alignas(4) glm::vec3 LightPos;
    alignas(4) glm::vec3 CameraPos;
};

int main(int argc, char** argv)
{
    dusk::App app(argc, argv);

    dusk::ShaderProgram sp({ "assets/shaders/Textured/default.fs.glsl", "assets/shaders/Textured/default.vs.glsl" });

    dusk::Model model;
    model.AddMesh(std::shared_ptr<dusk::Mesh>(new dusk::Mesh("assets/models/globe/globe.obj")));

    dusk::Box bounds = model.GetBounds();

    dusk::Camera camera;
    camera.SetAspect(app.GetWindowSize());
    camera.SetPosition(bounds.Max + (bounds.GetSize() * 0.5f));
    camera.SetForward(bounds.Min - bounds.Max);

    LightingData lightData;
    lightData.LightPos = lightData.CameraPos = camera.GetPosition();

    sp.Bind();
    dusk::ShaderProgram::SetUniformBufferData("LightingData", &lightData);

    app.EvtWindowResize.AddStatic([&](glm::ivec2 size) {
        camera.SetAspect(size);
    });

    app.EvtUpdate.AddStatic([&](const dusk::UpdateContext& ctx) {
        glm::vec3 rot = model.GetRotation();
        rot.y += glm::radians(0.5f * ctx.DeltaTime);
        model.SetRotation(rot);

        model.Update(ctx);
    });

    app.EvtRender.AddStatic([&](dusk::RenderContext& ctx) {
        ctx.CurrentShader = &sp;
        ctx.CurrentCamera = &camera;
        model.Render(ctx);
    });

    app.Start();
    return 0;
}
