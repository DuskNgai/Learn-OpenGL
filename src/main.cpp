#include <example-layer.hpp>
#include <light-layer.hpp>
#include <advanced-layer.hpp>
#include <sandbox.hpp>

int main(int argc, char** argv) {
    dusk::Log::init();
    DUSK_CORE_INFO("Initialized Log.");

    // TODO: argparser
    dusk::Layer* layer = nullptr;
    if (argc == 1) {
        layer = new learngl::ExampleLayer();
    }
    else {
        std::string option{ argv[1] };
        if (option == "example")
            layer = new learngl::ExampleLayer();
        else if (option == "light")
            layer = new learngl::LightLayer();
        else if (option == "advanced")
            layer = new learngl::AdvancedLayer();
        else {
            DUSK_CORE_ERROR("Unknown option: {0}", option);
            return -1;
        }
    }
    learngl::Sandbox* app = new learngl::Sandbox();
    app->push_layer(layer);
    app->run();
    delete app;

    return 0;
}
