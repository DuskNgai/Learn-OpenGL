#include <imgui/imgui.h>

#include <ExampleLayer.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

ExampleLayer::ExampleLayer()
    : Layer("ExampleLayer") {}

void ExampleLayer::OnAttach() {
    // A shader library for managing shaders.
    this->m_shader_library = std::make_unique<Dusk::ShaderLibrary>();
    // Camera track ball.
    this->m_trackball = std::make_unique<Dusk::TrackBall>(Dusk::Camera::Create(Dusk::CameraType::Perspective));
    // Tetrahedron.
    {
        // clang-format off
        float vertices[] = {
            // Position(3), Color(4)
             1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        };
        uint32_t indices[] = {0, 1, 2, 0, 1, 3, 0, 2, 3, 1, 2, 3};
        // clang-format on

        this->m_vertex_array = Dusk::VertexArray::Create();
        auto vertex_buffer = Dusk::VertexBuffer::Create(sizeof(vertices), vertices);
        vertex_buffer->SetLayout(Dusk::BufferLayout({
            {Dusk::ShaderDataType::Vec3, "a_Position"},
            {Dusk::ShaderDataType::Vec4,    "a_Color"}
        }));
        this->m_vertex_array->AddVertexBuffer(vertex_buffer);

        auto index_buffer = Dusk::IndexBuffer::Create(sizeof(indices) / sizeof(uint32_t), indices);
        this->m_vertex_array->SetIndexBuffer(index_buffer);

        // Shader for tetrahedron.
        this->m_shader_library->Add(
            "First",
            Dusk::Shader::Create(
                Dusk::ReadTextFile(Dusk::GetFilePath("assets/shaders/FirstVertexShader.glsl")),
                Dusk::ReadTextFile(Dusk::GetFilePath("assets/shaders/FirstFragmentShader.glsl"))
            )
        );
    }
    // UV map.
    {
        // clang-format off
        float uv_vertices[] = {
            // Position(3), Color(4), TexCoord(2), TexIndex(1), TilingScale(1)
            -1.0f, -1.0f, -1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -1.0f,  1.0f, -1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
             1.0f,  1.0f, -1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
             1.0f, -1.0f, -1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        };
        uint32_t uv_indices[] = {0, 1, 2, 0, 2, 3};
        // clang-format on

        this->m_texture_vertex_array = Dusk::VertexArray::Create();
        auto vertex_buffer = Dusk::VertexBuffer::Create(sizeof(uv_vertices), uv_vertices);
        vertex_buffer->SetLayout(Dusk::BufferLayout({
            { Dusk::ShaderDataType::Vec3,    "a_Position"},
            { Dusk::ShaderDataType::Vec4,       "a_Color"},
            { Dusk::ShaderDataType::Vec2,    "a_TexCoord"},
            {Dusk::ShaderDataType::Float,    "a_TexIndex"},
            {Dusk::ShaderDataType::Float, "a_TilingScale"}
        }));
        this->m_texture_vertex_array->AddVertexBuffer(vertex_buffer);

        auto index_buffer = Dusk::IndexBuffer::Create(sizeof(uv_indices) / sizeof(uint32_t), uv_indices);
        this->m_texture_vertex_array->SetIndexBuffer(index_buffer);

        // Shader for texture quad.
        this->m_shader_library->Add(
            "Texture",
            Dusk::Shader::Create(
                Dusk::ReadTextFile(Dusk::GetFilePath("assets/shaders/TextureVertexShader.glsl")),
                Dusk::ReadTextFile(Dusk::GetFilePath("assets/shaders/TextureFragmentShader.glsl"))
            )
        );
        // Bindings uniform variable to shader.
        this->m_shader_library->Get("Texture")->Bind();
        this->m_shader_library->Get("Texture")->SetVec4("u_Color", {1.0f, 1.0f, 1.0f, 1.0f});
        auto sampler = Dusk::IntegerSequenceToArray(std::make_integer_sequence<int, 32>{});
        this->m_shader_library->Get("Texture")->SetIntArray("u_Textures", sampler.data(), 32);
    }
    // Texture.
    {
        this->m_texture = Dusk::Texture2D::Create(Dusk::GetFilePath("assets/images/cornell.png"));
        this->m_alpha_texture = Dusk::Texture2D::Create(Dusk::GetFilePath("assets/images/hexagram.png"));
    }
    // Framebuffer.
    {
        Dusk::FrameBufferProps props{1280, 720};
        this->m_frame_buffer = Dusk::FrameBuffer::Create(props);
    }
}

void ExampleLayer::OnDetach() {}

void ExampleLayer::OnImGuiRender() {

    static bool np_open = true;
    static bool *p_open = &np_open;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", p_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Flag: NoSplit",                "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
            if (ImGui::MenuItem("Flag: NoResize",               "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
            if (ImGui::MenuItem("Flag: AutoHideTabBar",         "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            if (ImGui::MenuItem("Flag: PassthruCentralNode",    "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();

            if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
                *p_open = false;
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::End();

    ImGuiViewport const* main_viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 30, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver);

    ImGui::Begin("Test for ImGui.");

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::ColorEdit4("BG color", glm::value_ptr(this->m_bg_color));

    static int camera_type = (int)Dusk::CameraType::Perspective;
    ImGui::Combo("Camera Type", (int*)&camera_type, "Orthographic Camera\0Perspective Camera\0");
    this->m_trackball->SetCameraType((Dusk::CameraType)camera_type);

    ImGui::Image((ImTextureID)(std::size_t)this->m_frame_buffer->GetColorAttachment(), ImVec2(1280, 720));

    ImGui::End();
}

void ExampleLayer::OnUpdate() {
    this->m_frame_buffer->Bind();
    Dusk::RenderCommand::Clear();
    Dusk::RenderCommand::SetClearColor(this->m_bg_color);

    this->m_trackball->OnUpdate();
    {
        Dusk::Renderer::BeginScene(this->m_trackball->GetCamera());

        static auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
        for (int y = -1; y <= 1; ++y) {
            for (int x = -1; x <= 1; ++x) {
                glm::vec3 translate{1.0f * x, 1.0f * y, 0.0f};
                glm::mat4 model = glm::translate(glm::mat4(1.0), translate) * scale;
                Dusk::Renderer::Submit(this->m_shader_library->Get("First").get(), this->m_vertex_array.get(), model);
            }
        }

        this->m_texture->Bind(0);
        Dusk::Renderer::Submit(this->m_shader_library->Get("Texture").get(), this->m_texture_vertex_array.get(), glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
        this->m_alpha_texture->Bind(0);
        Dusk::Renderer::Submit(this->m_shader_library->Get("Texture").get(), this->m_texture_vertex_array.get(), glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

        Dusk::Renderer::EndScene();
    }
    {
        Dusk::Renderer2D::BeginScene(this->m_trackball->GetCamera());

        static float rotation = 0.0f;

        rotation += 5.0f * Dusk::Timer::GetDeltaTime();
        // Pure color.
        Dusk::Renderer2D::DrawQuad({-1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.5f, 0.5f, 1.0f});
        Dusk::Renderer2D::DrawQuad({1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 0.5f, 1.0f});
        Dusk::Renderer2D::DrawRotatedQuad({-1.0f, -1.0f}, {1.0f, 1.0f}, glm::radians(rotation), {0.5f, 1.0f, 0.5f, 1.0f});
        Dusk::Renderer2D::DrawRotatedQuad({1.0f, -1.0f}, {1.0f, 1.0f}, glm::radians(rotation), {0.5f, 0.5f, 1.0f, 1.0f});
        // Texture.
        Dusk::Renderer2D::DrawQuad({0.0f, 1.0f}, {1.0f, 1.0f}, this->m_texture);
        Dusk::Renderer2D::DrawRotatedQuad({0.0f, -1.0f}, {1.0f, 1.0f}, glm::radians(rotation), this->m_texture);
        Dusk::Renderer2D::DrawQuad({1.0f, 0.0f}, {1.0f, 1.0f}, this->m_alpha_texture);
        Dusk::Renderer2D::DrawRotatedQuad({-1.0f, 0.0f}, {1.0f, 1.0f}, glm::radians(rotation), this->m_alpha_texture);

        Dusk::Renderer2D::EndScene();
    }
    this->m_frame_buffer->Unbind();
}

void ExampleLayer::OnEvent(Dusk::EventBase& e) {
    this->m_trackball->OnEvent(e);
}

LEARN_OPENGL_NAMESPACE_END
