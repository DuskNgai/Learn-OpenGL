#include <imgui/imgui.h>

#include <LightLayer.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

LightLayer::LightLayer()
    : Layer("LightLayer") {}

void LightLayer::OnAttach() {
    // Camera track ball.
    this->m_trackball = std::make_unique<Dusk::TrackBall>(Dusk::Camera::Create(Dusk::CameraType::Perspective));
    // Cube.
    {
        // clang-format off
        float vertices[] = {
            // Position(3), Normal(3), TexCoord(2)
            // Back
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
            // Front
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
            // Left
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
            // Right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f, -1.0f, 1.0f, 0.0f,
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f, -1.0f, 1.0f, 1.0f,
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f, -1.0f, 0.0f, 1.0f,
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f, -1.0f, 0.0f, 0.0f,
            // Bottom
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f, -1.0f, 0.0f, 1.0f,
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f, -1.0f, 1.0f, 1.0f,
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f, -1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f, -1.0f, 0.0f, 0.0f,
            // Top
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f, -1.0f, 0.0f, 1.0f,
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f, -1.0f, 1.0f, 1.0f,
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f, -1.0f, 1.0f, 0.0f,
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f, -1.0f, 0.0f, 0.0f,
        };
        uint32_t indices[] = {
             0,  1,  2,  0,  2,  3,
             4,  5,  6,  4,  6,  7,
             8,  9, 10,  8, 10, 11,
            12, 13, 14, 12, 14, 15,
            16, 17, 18, 16, 18, 19,
            20, 21, 22, 20, 22, 23
        };
        // clang-format on

        this->m_cube_vao = Dusk::VertexArray::Create();
        auto vertex_buffer = Dusk::VertexBuffer::Create(sizeof(vertices), vertices);
        vertex_buffer->SetLayout(Dusk::BufferLayout({
            {Dusk::ShaderDataType::Vec3, "a_Position"},
            {Dusk::ShaderDataType::Vec3,   "a_Normal"},
            {Dusk::ShaderDataType::Vec2, "a_TexCoord"}
        }));
        this->m_cube_vao->AddVertexBuffer(vertex_buffer);

        auto index_buffer = Dusk::IndexBuffer::Create(sizeof(indices) / sizeof(uint32_t), indices);
        this->m_cube_vao->SetIndexBuffer(index_buffer);

        this->m_light_cube_vao = Dusk::VertexArray::Create();
        this->m_light_cube_vao->AddVertexBuffer(vertex_buffer);
        this->m_light_cube_vao->SetIndexBuffer(index_buffer);
    }
    // A shader library for managing shaders.
    {
        this->m_shader_library = std::make_unique<Dusk::ShaderLibrary>();
        this->m_shader_library->Add(
            "Cube",
            Dusk::Shader::Create(
                Dusk::ReadTextFile(Dusk::GetFilePath("assets/shaders/lighting/LightingVertexShader.glsl")),
                Dusk::ReadTextFile(Dusk::GetFilePath("assets/shaders/lighting/CubeFragmentShader.glsl"))
            )
        );
        this->m_shader_library->Get("Cube")->Bind();
        this->m_shader_library->Get("Cube")->SetVec3("u_ObjectColor", glm::vec3(1.0f, 0.5f, 0.31f));
        this->m_shader_library->Get("Cube")->SetVec3("u_LightColor", glm::vec3(1.0f, 1.0f, 1.0f));

        this->m_shader_library->Add(
            "Light",
            Dusk::Shader::Create(
                Dusk::ReadTextFile(Dusk::GetFilePath("assets/shaders/lighting/LightingVertexShader.glsl")),
                Dusk::ReadTextFile(Dusk::GetFilePath("assets/shaders/lighting/LightingFragmentShader.glsl"))
            )
        );
    }
    // Texture.
    {
    }
    // Framebuffer.
    {
        Dusk::FrameBufferProps props;
        props.Width = 1280;
        props.Height = 720;
        props.Samples = 1;
        this->m_frame_buffer = Dusk::FrameBuffer::Create(props);
    }
}

void LightLayer::OnDetach() {}

void LightLayer::OnImGuiRender() {
    ImGuiViewport const* main_viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 30, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver);

    ImGui::Begin("Test for ImGui.");

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::ColorEdit4("BG color", glm::value_ptr(this->m_bg_color));

    {
        static int camera_type = (int)Dusk::CameraType::Perspective;
        ImGui::Combo("Camera Type", (int*)&camera_type, "Orthographic Camera\0Perspective Camera\0");
        this->m_trackball->SetCameraType((Dusk::CameraType)camera_type);
    }
    ImGui::DragFloat3("Light Position", glm::value_ptr(this->m_light_pos), 0.05f);

    ImGui::End();
}

void LightLayer::OnUpdate() {
    Dusk::RenderCommand::Clear();
    Dusk::RenderCommand::SetClearColor(this->m_bg_color);

    this->m_trackball->OnUpdate();

    Dusk::Renderer::BeginScene(this->m_trackball->GetCamera());

    this->m_shader_library->Get("Cube")->Bind();
    this->m_shader_library->Get("Cube")->SetVec3("u_LightPos", this->m_light_pos);
    Dusk::Renderer::Submit(this->m_shader_library->Get("Cube"), this->m_cube_vao.get(), glm::mat4(1.0f));

    auto model = glm::translate(glm::mat4(1.0f), this->m_light_pos) * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
    Dusk::Renderer::Submit(this->m_shader_library->Get("Light"), this->m_light_cube_vao.get(), model);

    Dusk::Renderer::EndScene();
}

void LightLayer::OnEvent(Dusk::EventBase& e) {
    this->m_trackball->OnEvent(e);
}

LEARN_OPENGL_NAMESPACE_END
