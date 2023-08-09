#include <imgui.h>

#include <example-layer.hpp>
#include <simple-shapes.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

ExampleLayer::ExampleLayer()
    : Layer{ "ExampleLayer" } {}

void ExampleLayer::on_attach() {
    // Camera track ball.
    this->m_trackball = std::make_unique<dusk::TrackBall>(dusk::Camera::create(dusk::CameraType::Perspective));
    {
        this->m_scene = std::make_unique<dusk::Scene>();
        this->m_entity.emplace("square", this->m_scene->create_entity());
        this->m_entity["square"].add_component<dusk::TransformComponent>();
        this->m_entity["square"].add_component<dusk::SpriteComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
        this->m_entity.emplace("camera", this->m_scene->create_entity());
        this->m_entity["camera"].add_component<dusk::TransformComponent>();
        this->m_entity["camera"].add_component<dusk::CameraComponent>(this->m_trackball->get_camera());
    }
    // Tetrahedron.
    {
        this->m_vertex_array.emplace("tetrahedron", dusk::VertexArray::create());
        auto vertex_buffer{ dusk::VertexBuffer::create(sizeof(TETRAHEDRON_VERTICES), TETRAHEDRON_VERTICES.data()) };
        vertex_buffer->set_layout(dusk::BufferLayout({
            {dusk::ShaderDataType::Vec3, "a_Position"},
            {dusk::ShaderDataType::Vec4,    "a_Color"}
        }));
        this->m_vertex_array["tetrahedron"]->add_vertex_buffer(vertex_buffer);

        auto index_buffer{ dusk::IndexBuffer::create(sizeof(TETRAHEDRON_INDICES), TETRAHEDRON_INDICES.data()) };
        this->m_vertex_array["tetrahedron"]->set_index_buffer(index_buffer);
    }
    // UV map.
    {
        this->m_vertex_array.emplace("texture", dusk::VertexArray::create());
        auto vertex_buffer{ dusk::VertexBuffer::create(sizeof(UV_VERTICES), UV_VERTICES.data()) };
        vertex_buffer->set_layout(dusk::BufferLayout({
            { dusk::ShaderDataType::Vec3,    "a_Position"},
            { dusk::ShaderDataType::Vec4,       "a_Color"},
            { dusk::ShaderDataType::Vec2,    "a_TexCoord"},
            {dusk::ShaderDataType::Float,    "a_TexIndex"},
            {dusk::ShaderDataType::Float, "a_TilingScale"}
        }));
        this->m_vertex_array["texture"]->add_vertex_buffer(vertex_buffer);

        auto index_buffer{ dusk::IndexBuffer::create(sizeof(UV_INDICES), UV_INDICES.data()) };
        this->m_vertex_array["texture"]->set_index_buffer(index_buffer);
    }
    // A shader library for managing shaders.
    {
        // Shader for tetrahedron.
        this->m_shader.emplace(
            "First",
            dusk::Shader::create(
                dusk::read_text_file(dusk::get_file_path("assets/shaders/FirstVS.glsl")),
                dusk::read_text_file(dusk::get_file_path("assets/shaders/FirstFS.glsl"))
            )
        );
        // Shader for texture quad.
        this->m_shader.emplace(
            "Texture",
            dusk::Shader::create(
                dusk::read_text_file(dusk::get_file_path("assets/shaders/TextureVS.glsl")),
                dusk::read_text_file(dusk::get_file_path("assets/shaders/TextureFS.glsl"))
            )
        );
        // Bindings uniform variable to shader.
        auto sampler{ dusk::IntegerSequenceToArray(std::make_integer_sequence<int, 32>{}) };
        this->m_shader["Texture"]->bind();
        this->m_shader["Texture"]->set_int_array("u_Textures", sampler.data(), sampler.size());
    }
    // Texture.
    {
        this->m_texture.emplace("awesomeface", dusk::Texture2D::create(dusk::get_file_path("assets/images/awesomeface.png")));
        this->m_texture.emplace("alpha", dusk::Texture2D::create(dusk::get_file_path("assets/images/blending_transparent_window.png")));
    }
    // Framebuffer.
    {
        auto window{ dusk::Application::get()->get_window() };
        this->m_frame_buffer = dusk::Framebuffer::create(window->get_width(), window->get_height());
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
    ImGui::ColorEdit4("FG color", glm::value_ptr(this->m_entity["square"].get_component<dusk::SpriteComponent>().color));

    // Set camera.
    static std::size_t camera_type_idx{ (std::size_t)dusk::CameraType::Perspective };
    static std::array<char const*, 2> camera_type{ "Orthographic Camera", "Perspective Camera" };
    if (ImGui::BeginCombo("Camera Type", camera_type[camera_type_idx])) {
        for (auto i : dusk::range(camera_type.size())) {
            bool is_selected{ (camera_type_idx == i) };
            if (ImGui::Selectable(camera_type[i], is_selected)) {
                camera_type_idx = i;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    this->m_trackball->set_camera_type((dusk::CameraType)camera_type_idx);

    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 30, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport");

    // Receiving input on focused.
    this->m_is_viewport_focused = ImGui::IsWindowFocused();
    this->m_is_viewport_hovered = ImGui::IsWindowHovered();
    dusk::Application::get()->get_imgui_layer()->set_block_event(not(this->m_is_viewport_focused and this->m_is_viewport_hovered));

    // Resize the viewport.
    auto viewport_size{ ImGui::GetContentRegionAvail() };
    if (this->m_viewport_size != glm::vec2{ viewport_size.x, viewport_size.y }) {
        this->m_viewport_size = { viewport_size.x, viewport_size.y };
        this->m_frame_buffer->resize(this->m_viewport_size.x, this->m_viewport_size.y);
        this->m_trackball->set_aspect_ratio(this->m_viewport_size.x / this->m_viewport_size.y);
    }
    ImGui::Image((ImTextureID)(std::size_t)this->m_frame_buffer->get_color_attachment(0)->get_texture_id(), { this->m_viewport_size.x, this->m_viewport_size.y }, { 0, 1 }, { 1, 0 });

    ImGui::End();
    ImGui::PopStyleVar(1);
}

void ExampleLayer::on_update() {
    dusk::RenderCommand::clear();
    dusk::RenderCommand::set_clear_color(this->m_bg_color);

    this->m_frame_buffer->bind();
    dusk::RenderCommand::clear();
    dusk::RenderCommand::set_clear_color(this->m_bg_color);
    dusk::RenderCommand::set_viewport(0, 0, this->m_viewport_size.x, this->m_viewport_size.y);

    if (this->m_is_viewport_focused) {
        this->m_trackball->on_update();
    }
    {
        dusk::Renderer::begin_scene(this->m_trackball->get_camera());

        static auto scale{ glm::scale(glm::mat4(1.0f), glm::vec3(0.2f)) };
        for (int y{ -1 }; y <= 1; ++y) {
            for (int x{ -1 }; x <= 1; ++x) {
                glm::vec3 translate{ 1.0f * x, 1.0f * y, 0.0f };
                glm::mat4 model{ glm::translate(glm::mat4(1.0f), translate) * scale };
                dusk::Renderer::submit(this->m_shader["First"], this->m_vertex_array["tetrahedron"], model);
            }
        }

        this->m_texture["awesomeface"]->bind(0);
        dusk::Renderer::submit(this->m_shader["Texture"], this->m_vertex_array["texture"], glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.5f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
        this->m_texture["alpha"]->bind(0);
        dusk::Renderer::submit(this->m_shader["Texture"], this->m_vertex_array["texture"], glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.5f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

        dusk::Renderer::end_scene();
    }
    this->m_scene->on_update();
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
        dusk::Renderer2D::draw_quad({ 0.0f, 1.0f }, { 1.0f, 1.0f }, this->m_texture["awesomeface"]);
        dusk::Renderer2D::draw_rotated_quad({ 0.0f, -1.0f }, { 1.0f, 1.0f }, glm::radians(rotation), this->m_texture["awesomeface"]);
        dusk::Renderer2D::draw_quad({ 1.0f, 0.0f }, { 1.0f, 1.0f }, this->m_texture["alpha"]);
        dusk::Renderer2D::draw_rotated_quad({ -1.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), this->m_texture["alpha"]);

        dusk::Renderer2D::end_scene();
    }
    this->m_frame_buffer->unbind();
}

void ExampleLayer::on_event(dusk::EventBase& e) {
    dusk::EventDispatcher dispatcher{ e };
    dispatcher.dispatch<dusk::WindowResizeEvent>(DUSK_BIND_CLASS_FN(ExampleLayer::on_window_resize));
    if (this->m_is_viewport_focused) {
        this->m_trackball->on_event(e);
    }
}

bool ExampleLayer::on_window_resize([[maybe_unused]] dusk::WindowResizeEvent& e) {
    return true;
}

LEARN_OPENGL_NAMESPACE_END
