#include <ExampleLayer.hpp>
#include <LightLayer.hpp>
#include <Sandbox.hpp>

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {

    Dusk::Log::Initialize();
    DUSK_CORE_INFO("Initialized Log.");

    LearnGL::Sandbox* app = new LearnGL::Sandbox();
    // app->PushLayer(new LearnGL::ExampleLayer());
    app->PushLayer(new LearnGL::LightLayer());
    app->Run();
    delete app;

    return 0;
}
