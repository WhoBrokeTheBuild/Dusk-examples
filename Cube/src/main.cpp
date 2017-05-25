#include <dusk/Dusk.hpp>

int main(int argc, char** argv)
{
    dusk::App app(argc, argv);
    app.LoadConfig("assets/config/Cube-example.json");
    app.Run();
    return 0;
}
