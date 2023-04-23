#include <example-layer.hpp>
#include <light-layer.hpp>
#include <sandbox.hpp>

int main(int argc, char** argv) {
    dusk::Log::init();
    DUSK_CORE_INFO("Initialized Log.");

    learngl::Sandbox* app = new learngl::Sandbox();
    // TODO: argparser
    if (argc == 1) {
        app->push_layer(new learngl::ExampleLayer());
    }
    else {
        std::string option { argv[1] };
        if (option== "example")
            app->push_layer(new learngl::ExampleLayer());
        else if (option== "light")
            app->push_layer(new learngl::LightLayer());
    }
    app->run();
    delete app;

    return 0;
}
