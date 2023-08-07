#include <imgui.h>

#include <advanced-layer.hpp>
#include <simple-shapes.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

AdvancedLayer::AdvancedLayer()
    : Layer{ "AdvancedLayer" } {}

void AdvancedLayer::on_attach() {
    // Camera track ball.
    this->m_trackball = std::make_unique<dusk::TrackBall>(dusk::Camera::create(dusk::CameraType::Perspective));
    // Cubes that many tasks needs
    {
        this->m_vao.emplace("cube", dusk::VertexArray::create());
        auto vertex_buffer{ dusk::VertexBuffer::create(sizeof(CUBE_VERTICES), CUBE_VERTICES.data()) };
        vertex_buffer->set_layout(dusk::BufferLayout({
            {dusk::ShaderDataType::Vec3, "a_Position"},
            {dusk::ShaderDataType::Vec3,   "a_Normal"},
            {dusk::ShaderDataType::Vec2, "a_TexCoord"}
        }));
        this->m_vao["cube"]->add_vertex_buffer(vertex_buffer);
        auto index_buffer{ dusk::IndexBuffer::create(sizeof(CUBE_INDICES), CUBE_INDICES.data()) };
        this->m_vao["cube"]->set_index_buffer(index_buffer);

        this->m_vao.emplace("plane", dusk::VertexArray::create());
        vertex_buffer = dusk::VertexBuffer::create(sizeof(PLANE_VERTICES), PLANE_VERTICES.data());
        vertex_buffer->set_layout(dusk::BufferLayout({
            {dusk::ShaderDataType::Vec3, "a_Position"},
            {dusk::ShaderDataType::Vec3,   "a_Normal"},
            {dusk::ShaderDataType::Vec2, "a_TexCoord"}
        }));
        this->m_vao["plane"]->add_vertex_buffer(vertex_buffer);
        index_buffer = dusk::IndexBuffer::create(sizeof(PLANE_INDICES), PLANE_INDICES.data());
        this->m_vao["plane"]->set_index_buffer(index_buffer);
    }
    // Textures that many tasks needs
    {
        this->m_texture.emplace("container", dusk::Texture2D::create(dusk::get_file_path("assets/images/container.jpg")));
        this->m_texture.emplace("container2", dusk::Texture2D::create(dusk::get_file_path("assets/images/container2.png")));
        this->m_texture.emplace("awesomeface", dusk::Texture2D::create(dusk::get_file_path("assets/images/awesomeface.png")));
        this->m_texture.emplace("grass", dusk::Texture2D::create(dusk::get_file_path("assets/images/grass.png")));
        this->m_texture.emplace("window", dusk::Texture2D::create(dusk::get_file_path("assets/images/blending_transparent_window.png")));
        // this->m_texture.emplace("cubemap", dusk::TextureCube::create(dusk::get_file_path("assets/images/skybox")));
    }
    this->depth_testing_on_attach();
    this->blending_on_attach();
    this->framebuffer_on_attach();

    this->m_task = "Blending";
}

void AdvancedLayer::on_detach() {}

void AdvancedLayer::on_ImGui_render() {
    ImGuiViewport const* main_viewport{ ImGui::GetMainViewport() };

    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 30, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver);

    ImGui::Begin("Test for ImGui.");

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::ColorEdit4("BG color", glm::value_ptr(this->m_bg_color));

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

    if (ImGui::BeginCombo("Task", this->m_task.c_str())) {
        for (auto const& [key, shader] : this->m_shader) {
            bool is_selected{ (this->m_task == key) };
            if (ImGui::Selectable(key.c_str(), is_selected)) {
                this->m_task = key;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    ImGui::End();
}

void AdvancedLayer::on_update() {
    dusk::RenderCommand::clear();
    dusk::RenderCommand::set_clear_color(this->m_bg_color);

    this->m_trackball->on_update();

    dusk::Renderer::begin_scene(this->m_trackball->get_camera());

    if (this->m_task == "DepthTest" or this->m_task == "Blending") {
        this->depth_testing_on_update();
        this->blending_on_update();
    }
    else if (this->m_task == "Framebuffer") {
        this->framebuffer_on_update();
    }

    dusk::Renderer::end_scene();
}

void AdvancedLayer::on_event(dusk::EventBase& e) {
    this->m_trackball->on_event(e);
}

void AdvancedLayer::depth_testing_on_attach() {
    this->m_shader.emplace(
        "DepthTest",
        dusk::Shader::create(
            dusk::read_text_file(dusk::get_file_path("assets/shaders/advanced/DepthTestVS.glsl")),
            dusk::read_text_file(dusk::get_file_path("assets/shaders/advanced/DepthTestFS.glsl"))
        )
    );
}

void AdvancedLayer::blending_on_attach() {
    this->m_shader.emplace(
        "Blending",
        dusk::Shader::create(
            dusk::read_text_file(dusk::get_file_path("assets/shaders/advanced/DepthTestVS.glsl")),
            dusk::read_text_file(dusk::get_file_path("assets/shaders/advanced/BlendingFS.glsl"))
        )
    );
    this->m_shader["Blending"]->bind();
    this->m_shader["Blending"]->set_int("u_Texture", 0);

    this->vegetation = {
        {-1.5f, 0.0f, -0.48f},
        { 1.5f, 0.0f,  0.51f},
        { 0.0f, 0.0f,   0.7f},
        {-0.3f, 0.0f,  -2.3f},
        { 0.5f, 0.0f,  -0.6f},
    };
}

void AdvancedLayer::framebuffer_on_attach() {
    this->m_shader.emplace(
        "Framebuffer",
        dusk::Shader::create(
            dusk::read_text_file(dusk::get_file_path("assets/shaders/advanced/DepthTestVS.glsl")),
            dusk::read_text_file(dusk::get_file_path("assets/shaders/advanced/FramebufferFS.glsl"))
        )
    );
    this->m_shader["Framebuffer"]->bind();
    this->m_shader["Framebuffer"]->set_int("u_Texture", 0);

    auto window{ dusk::Application::get()->get_window() };
    this->m_framebuffer = dusk::Framebuffer::create(window->get_width(), window->get_height());
}

void AdvancedLayer::depth_testing_on_update() {
    auto selected_shader{ this->m_shader[this->m_task] };
    if (this->m_task == "DepthTest") {
        selected_shader->bind();
        selected_shader->set_float("u_Near", this->m_trackball->get_near_plane());
        selected_shader->set_float("u_Far", this->m_trackball->get_far_plane());
    }

    this->m_texture["container"]->bind(0);
    dusk::Renderer::submit(selected_shader, this->m_vao["cube"], glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, -1.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));
    this->m_texture["container2"]->bind(0);
    dusk::Renderer::submit(selected_shader, this->m_vao["cube"], glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));
    this->m_texture["awesomeface"]->bind(0);
    dusk::Renderer::submit(selected_shader, this->m_vao["plane"], glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(4.0f)));
}

void AdvancedLayer::blending_on_update() {
    auto selected_shader{ this->m_shader[this->m_task] };

    // Draw the texture by their distance (far to near) to the camera.
    std::sort(this->vegetation.begin(), this->vegetation.end(), [&](glm::vec3 const& lhs, glm::vec3 const& rhs) {
        auto&& target{ this->m_trackball->get_look_from() };
        return glm::length(target - lhs) > glm::length(target - rhs);
    });
    for (auto i : dusk::range(this->vegetation.size())) {
        auto model = glm::translate(glm::mat4(1.0f), vegetation[i] + glm::vec3(0.5f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
        this->m_texture["grass"]->bind(0);
        dusk::Renderer::submit(selected_shader, this->m_vao["plane"], model);
        this->m_texture["window"]->bind(0);
        dusk::Renderer::submit(selected_shader, this->m_vao["plane"], model);
    }
}

void AdvancedLayer::framebuffer_on_update() {
    // Draw the framebuffer.
    this->m_framebuffer->bind();
    dusk::RenderCommand::clear();
    dusk::RenderCommand::set_clear_color({ 0.1f, 0.1f, 0.1f, 1.0f });
    dusk::RenderCommand::enable_render_test(dusk::RenderTest::DepthTest);
    this->depth_testing_on_update();
    this->m_framebuffer->unbind();

    // Draw the screen.
    this->m_shader["Blending"]->bind();
    this->m_framebuffer->get_color_attachment(0)->bind(0);
    dusk::Renderer::submit(this->m_shader["Blending"], this->m_vao["plane"], glm::scale(glm::mat4(1.0f), glm::vec3(4.0f, 2.25f, 1.0f)));
}

LEARN_OPENGL_NAMESPACE_END
