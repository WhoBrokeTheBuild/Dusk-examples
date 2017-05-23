#include <dusk/Dusk.hpp>

int main(int argc, char** argv)
{
    dusk::App * app = new dusk::App(argc, argv);
    app->Run();
    delete app;

    return 0;
}
