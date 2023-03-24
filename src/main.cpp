#include <example-layer.hpp>
#include <light-layer.hpp>
#include <sandbox.hpp>

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    dusk::Log::init();
    DUSK_CORE_INFO("Initialized Log.");

    learngl::Sandbox* app = new learngl::Sandbox();
    // app->push_layer(new learngl::ExampleLayer());
    app->push_layer(new learngl::LightLayer());
    app->run();
    delete app;

    return 0;
}
