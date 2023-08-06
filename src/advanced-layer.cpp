#include <imgui.h>

#include <advanced-layer.hpp>
#include <simple-shapes.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

AdvancedLayer::AdvancedLayer()
    : Layer{ "AdvancedLayer" } {}

void AdvancedLayer::on_attach() {
    // Camera track ball.
    this->m_trackball = std::make_unique<dusk::TrackBall>(dusk::Camera::create(dusk::CameraType::Perspective));
    // Cube.
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
    // A shader library for managing shaders.
    {
        this->m_shader_library = std::make_unique<dusk::ShaderLibrary>();
        this->m_shader_library->emplace(
            "DepthTest",
            dusk::Shader::create(
                dusk::read_text_file(dusk::get_file_path("assets/shaders/advanced/DepthTestVS.glsl")),
                dusk::read_text_file(dusk::get_file_path("assets/shaders/advanced/DepthTestFS.glsl"))
            )
        );
        this->m_shader_library->emplace(
            "Blending",
            dusk::Shader::create(
                dusk::read_text_file(dusk::get_file_path("assets/shaders/advanced/DepthTestVS.glsl")),
                dusk::read_text_file(dusk::get_file_path("assets/shaders/advanced/BlendingFS.glsl"))
            )
        );

        // Bindings uniform variable to shader.
        auto sampler{ dusk::IntegerSequenceToArray(std::make_integer_sequence<int, 1>{}) };
        this->m_shader_library->get("Blending")->bind();
        this->m_shader_library->get("Blending")->set_int_array("u_Textures", sampler.data(), sampler.size());
    }
    // Texture.
    {
        this->m_texture.emplace("container2", dusk::Texture2D::create(dusk::get_file_path("assets/images/container2.png")));
        this->m_texture.emplace("awesomeface", dusk::Texture2D::create(dusk::get_file_path("assets/images/awesomeface.png")));
        this->m_texture.emplace("grass", dusk::Texture2D::create(dusk::get_file_path("assets/images/grass.png")));
        this->m_texture.emplace("window", dusk::Texture2D::create(dusk::get_file_path("assets/images/blending_transparent_window.png")));
    }
    this->vegetation = {
        {-1.5f, 0.0f, -0.48f},
        { 1.5f, 0.0f,  0.51f},
        { 0.0f, 0.0f,   0.7f},
        {-0.3f, 0.0f,  -2.3f},
        { 0.5f, 0.0f,  -0.6f},
    };
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

    if (ImGui::BeginCombo("Shader Type", this->m_shader_library->get(this->m_shader_idx).first.c_str())) {
        for (auto i : dusk::range(this->m_shader_library->size())) {
            bool is_selected{ (this->m_shader_idx == i) };
            if (ImGui::Selectable(this->m_shader_library->get(i).first.c_str(), is_selected)) {
                this->m_shader_idx = i;
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

    auto named_shader = this->m_shader_library->get(this->m_shader_idx);
    if (named_shader.first == "DepthTest") {
        named_shader.second->bind();
        named_shader.second->set_float("u_Near", this->m_trackball->get_near_plane());
        named_shader.second->set_float("u_Far", this->m_trackball->get_far_plane());
    }

    this->m_texture["container2"]->bind(0);
    dusk::Renderer::submit(named_shader.second, this->m_vao["cube"], glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, -1.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));
    dusk::Renderer::submit(named_shader.second, this->m_vao["cube"], glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));

    this->m_texture["awesomeface"]->bind(0);
    auto model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));
    dusk::Renderer::submit(named_shader.second, this->m_vao["plane"], model);

    // Draw the texture by their distance (far to near) to the camera.
    std::sort(this->vegetation.begin(), this->vegetation.end(), [&](glm::vec3 const& lhs, glm::vec3 const& rhs) {
        auto&& target{ this->m_trackball->get_look_from() };
        return glm::length(target - lhs) > glm::length(target - rhs);
    });
    for (auto i : dusk::range(this->vegetation.size())) {
        model = glm::translate(glm::mat4(1.0f), vegetation[i] + glm::vec3(0.5f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
        this->m_texture["grass"]->bind(0);
        dusk::Renderer::submit(named_shader.second, this->m_vao["plane"], model);
        this->m_texture["window"]->bind(0);
        dusk::Renderer::submit(named_shader.second, this->m_vao["plane"], model);
    }

    dusk::Renderer::end_scene();
}

void AdvancedLayer::on_event(dusk::EventBase& e) {
    this->m_trackball->on_event(e);
}

LEARN_OPENGL_NAMESPACE_END
