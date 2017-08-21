#include <dusk/Dusk.hpp>

int main(int argc, char** argv)
{
    using namespace noise;
    
    dusk::App app(argc, argv);

    dusk::ShaderProgram sp({ "assets/shaders/Noise/default.fs.glsl", "assets/shaders/Noise/default.vs.glsl" });
    sp.Bind();

    dusk::Camera camera;
    camera.SetAspect(app.GetWindowSize());
    camera.SetPosition({ 0.0f, 0.0f, 2.0f });
    camera.SetForward({ 0.0f, 0.0f, -1.0f });

    const size_t SIZE = 128;

    std::vector<uint8_t> pixels(SIZE * SIZE * 3, 0);

    module::RidgedMulti primaryJade;
    primaryJade.SetSeed (0);
    primaryJade.SetFrequency (2.0);
    primaryJade.SetLacunarity (2.20703125);
    primaryJade.SetOctaveCount (6);
    primaryJade.SetNoiseQuality (QUALITY_STD);

    dusk::Texture tex;
    tex.LoadFromData({ SIZE, SIZE }, pixels, GL_RGB);
    //tex.LoadFromFile("assets/models/crate/crate.png");

    sp.SetUniform("u_Noise", 0);
    glActiveTexture(GL_TEXTURE0);
    tex.Bind();

    dusk::Mesh::Data meshData;
    meshData.Vertices = {
        { -1.0f, -1.0f, 0.0f },
        { -1.0f,  1.0f, 0.0f },
        {  1.0f, -1.0f, 0.0f },
        { -1.0f,  1.0f, 0.0f },
        {  1.0f, -1.0f, 0.0f },
        {  1.0f,  1.0f, 0.0f },
    };
    meshData.TexCoords = {
        { 0.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
    };

    dusk::Model model;
    model.AddMesh(std::shared_ptr<dusk::Mesh>(new dusk::Mesh(meshData)));

    app.EvtWindowResize.AddStatic([&](glm::ivec2 size) {
        camera.SetAspect(size);
    });

    app.EvtUpdate.AddStatic([&](const dusk::UpdateContext& ctx) {
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
