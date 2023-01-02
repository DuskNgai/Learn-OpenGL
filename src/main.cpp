#include <ExampleLayer.hpp>
#include <Sandbox.hpp>

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {

    Dusk::Log::Initialize();
    DUSK_CORE_INFO("Initialized Log.");

    LearnGL::Sandbox* app = new LearnGL::Sandbox();
    app->PushLayer(new LearnGL::ExampleLayer());
    app->Run();
    delete app;

    return 0;
}
