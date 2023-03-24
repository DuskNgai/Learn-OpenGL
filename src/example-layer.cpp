#include <imgui/imgui.h>

#include <example-layer.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

ExampleLayer::ExampleLayer()
    : Layer{ "ExampleLayer" } {}

void ExampleLayer::on_attach() {
    // A shader library for managing shaders.
    this->m_shader_library = std::make_unique<dusk::ShaderLibrary>();
    // Camera track ball.
    this->m_trackball = std::make_unique<dusk::TrackBall>(dusk::Camera::create(dusk::CameraType::Perspective));
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

        this->m_vertex_array = dusk::VertexArray::create();
        auto vertex_buffer{ dusk::VertexBuffer::create(sizeof(vertices), vertices) };
        vertex_buffer->set_layout(dusk::BufferLayout({
            {dusk::ShaderDataType::Vec3, "a_Position"},
            {dusk::ShaderDataType::Vec4,    "a_Color"}
        }));
        this->m_vertex_array->add_vertex_buffer(vertex_buffer);

        auto index_buffer{ dusk::IndexBuffer::create(sizeof(indices) / sizeof(uint32_t), indices) };
        this->m_vertex_array->set_index_buffer(index_buffer);

        // Shader for tetrahedron.
        this->m_shader_library->add(
            "First",
            dusk::Shader::create(
                dusk::read_text_file(dusk::get_file_path("assets/shaders/FirstVertexShader.glsl")),
                dusk::read_text_file(dusk::get_file_path("assets/shaders/FirstFragmentShader.glsl"))
            )
        );
    }
    // UV map.
    {
        // clang-format off
        float uv_vertices[] {
            // Position(3), Color(4), TexCoord(2), TexIndex(1), TilingScale(1)
            -1.0f, -1.0f, -1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -1.0f,  1.0f, -1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
             1.0f,  1.0f, -1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
             1.0f, -1.0f, -1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        };
        uint32_t uv_indices[] {0, 1, 2, 0, 2, 3};
        // clang-format on

        this->m_texture_vertex_array = dusk::VertexArray::create();
        auto vertex_buffer{ dusk::VertexBuffer::create(sizeof(uv_vertices), uv_vertices) };
        vertex_buffer->set_layout(dusk::BufferLayout({
            { dusk::ShaderDataType::Vec3,    "a_Position"},
            { dusk::ShaderDataType::Vec4,       "a_Color"},
            { dusk::ShaderDataType::Vec2,    "a_TexCoord"},
            {dusk::ShaderDataType::Float,    "a_TexIndex"},
            {dusk::ShaderDataType::Float, "a_TilingScale"}
        }));
        this->m_texture_vertex_array->add_vertex_buffer(vertex_buffer);

        auto index_buffer{ dusk::IndexBuffer::create(sizeof(uv_indices) / sizeof(uint32_t), uv_indices) };
        this->m_texture_vertex_array->set_index_buffer(index_buffer);

        // Shader for texture quad.
        this->m_shader_library->add(
            "Texture",
            dusk::Shader::create(
                dusk::read_text_file(dusk::get_file_path("assets/shaders/TextureVertexShader.glsl")),
                dusk::read_text_file(dusk::get_file_path("assets/shaders/TextureFragmentShader.glsl"))
            )
        );
        // Bindings uniform variable to shader.
        this->m_shader_library->get("Texture")->bind();
        this->m_shader_library->get("Texture")->set_vec4("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f });
        auto sampler{ dusk::IntegerSequenceToArray(std::make_integer_sequence<int, 32>{}) };
        this->m_shader_library->get("Texture")->set_int_array("u_Textures", sampler.data(), 32);
    }
    // Texture.
    {
        this->m_texture = dusk::Texture2D::create(dusk::get_file_path("assets/images/cornell.png"));
        this->m_alpha_texture = dusk::Texture2D::create(dusk::get_file_path("assets/images/hexagram.png"));
    }
    // Framebuffer.
    {
        dusk::FramebufferProps props{ 1280, 720 };
        this->m_frame_buffer = dusk::Framebuffer::create(props);
    }
}

void ExampleLayer::on_detach() {}

void ExampleLayer::on_ImGui_render() {
    static bool np_open = true;
    static bool* p_open = &np_open;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport const* viewport = ImGui::GetMainViewport();
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

            if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) {
                dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
            }
            if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) {
                dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
            }
            if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) {
                dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
            }
            if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) {
                dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
            }
            if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) {
                dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
            }
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

    static int camera_type = (int)dusk::CameraType::Perspective;
    ImGui::Combo("Camera Type", (int*)&camera_type, "Orthographic Camera\0Perspective Camera\0");
    this->m_trackball->set_camera_type((dusk::CameraType)camera_type);

    ImGui::Image((ImTextureID)(std::size_t)this->m_frame_buffer->get_color_attachment(), ImVec2(1280, 720));

    ImGui::End();
}

void ExampleLayer::on_update() {
    this->m_frame_buffer->bind();
    dusk::RenderCommand::clear();
    dusk::RenderCommand::set_clear_color(this->m_bg_color);

    this->m_trackball->on_update();
    {
        dusk::Renderer::begin_scene(this->m_trackball->get_camera());

        static auto scale{ glm::scale(glm::mat4(1.0f), glm::vec3(0.2f)) };
        for (int y{ -1 }; y <= 1; ++y) {
            for (int x{ -1 }; x <= 1; ++x) {
                glm::vec3 translate{ 1.0f * x, 1.0f * y, 0.0f };
                glm::mat4 model{ glm::translate(glm::mat4(1.0), translate) * scale };
                dusk::Renderer::submit(this->m_shader_library->get("First").get(), this->m_vertex_array.get(), model);
            }
        }

        this->m_texture->bind(0);
        dusk::Renderer::submit(this->m_shader_library->get("Texture").get(), this->m_texture_vertex_array.get(), glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
        this->m_alpha_texture->bind(0);
        dusk::Renderer::submit(this->m_shader_library->get("Texture").get(), this->m_texture_vertex_array.get(), glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

        dusk::Renderer::end_scene();
    }
    {
        dusk::Renderer2D::begin_scene(this->m_trackball->get_camera());

        static float rotation{ 0.0f };

        rotation += 5.0f * dusk::Timer::get_delta_time();
        // Pure color.
        dusk::Renderer2D::draw_quad({ -1.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.5f, 0.5f, 1.0f });
        dusk::Renderer2D::draw_quad({ 1.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 0.5f, 1.0f });
        dusk::Renderer2D::draw_rotated_quad({ -1.0f, -1.0f }, { 1.0f, 1.0f }, glm::radians(rotation), { 0.5f, 1.0f, 0.5f, 1.0f });
        dusk::Renderer2D::draw_rotated_quad({ 1.0f, -1.0f }, { 1.0f, 1.0f }, glm::radians(rotation), { 0.5f, 0.5f, 1.0f, 1.0f });
        // Texture.
        dusk::Renderer2D::draw_quad({ 0.0f, 1.0f }, { 1.0f, 1.0f }, this->m_texture);
        dusk::Renderer2D::draw_rotated_quad({ 0.0f, -1.0f }, { 1.0f, 1.0f }, glm::radians(rotation), this->m_texture);
        dusk::Renderer2D::draw_quad({ 1.0f, 0.0f }, { 1.0f, 1.0f }, this->m_alpha_texture);
        dusk::Renderer2D::draw_rotated_quad({ -1.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), this->m_alpha_texture);

        dusk::Renderer2D::end_scene();
    }
    this->m_frame_buffer->unbind();
}

void ExampleLayer::on_event(dusk::EventBase& e) {
    this->m_trackball->on_event(e);
}

LEARN_OPENGL_NAMESPACE_END