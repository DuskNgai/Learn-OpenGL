#include <imgui/imgui.h>

#include <light-layer.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

LightLayer::LightLayer()
    : Layer{ "LightLayer" } {}

void LightLayer::on_attach() {
    // Camera track ball.
    this->m_trackball = std::make_unique<dusk::TrackBall>(dusk::Camera::create(dusk::CameraType::Perspective));
    // Cube.
    {
        // clang-format off
        float vertices[] {
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
        uint32_t indices[] {
             0,  1,  2,  0,  2,  3,
             4,  5,  6,  4,  6,  7,
             8,  9, 10,  8, 10, 11,
            12, 13, 14, 12, 14, 15,
            16, 17, 18, 16, 18, 19,
            20, 21, 22, 20, 22, 23
        };
        // clang-format on

        this->m_cube_vao = dusk::VertexArray::create();

        auto vertex_buffer{ dusk::VertexBuffer::create(sizeof(vertices), vertices) };
        vertex_buffer->set_layout(dusk::BufferLayout({
            {dusk::ShaderDataType::Vec3, "a_Position"},
            {dusk::ShaderDataType::Vec3,   "a_Normal"},
            {dusk::ShaderDataType::Vec2, "a_TexCoord"}
        }));
        this->m_cube_vao->add_vertex_buffer(vertex_buffer);

        auto index_buffer{ dusk::IndexBuffer::create(sizeof(indices) / sizeof(uint32_t), indices) };
        this->m_cube_vao->set_index_buffer(index_buffer);

        this->m_light_cube_vao = dusk::VertexArray::create();
        this->m_light_cube_vao->add_vertex_buffer(vertex_buffer);
        this->m_light_cube_vao->set_index_buffer(index_buffer);
    }
    // A shader library for managing shaders.
    {
        this->m_shader_library = std::make_unique<dusk::ShaderLibrary>();
        this->m_shader_library->add(
            "PureColor",
            dusk::Shader::create(
                dusk::read_text_file(dusk::get_file_path("assets/shaders/lighting/ObjectVertexShader.glsl")),
                dusk::read_text_file(dusk::get_file_path("assets/shaders/lighting/PureColorFragmentShader.glsl"))
            )
        );
        this->m_shader_library->add(
            "Texture",
            dusk::Shader::create(
                dusk::read_text_file(dusk::get_file_path("assets/shaders/lighting/ObjectVertexShader.glsl")),
                dusk::read_text_file(dusk::get_file_path("assets/shaders/lighting/TextureFragmentShader.glsl"))
            )
        );
        this->m_shader_library->add(
            "Light",
            dusk::Shader::create(
                dusk::read_text_file(dusk::get_file_path("assets/shaders/lighting/ObjectVertexShader.glsl")),
                dusk::read_text_file(dusk::get_file_path("assets/shaders/lighting/LightFragmentShader.glsl"))
            )
        );
    }
    // Texture & Material & Light.
    {
        this->m_material = {
            {         "Corol",               PureColorMaterial::create(glm::vec3{ 1.0f, 0.5f, 0.31f },               glm::vec3{ 1.0f, 0.5f, 0.31f },                      glm::vec3{ 0.5f, 0.5f, 0.5f }, 32.0f)},
            {       "Emerald",       PureColorMaterial::create(glm::vec3{ 0.0215f, 0.1745f, 0.0215f },    glm::vec3{ 0.07568f, 0.61424f, 0.07568f },             glm::vec3{ 0.633f, 0.727811f, 0.633f }, 76.8f)},
            {          "Jade",        PureColorMaterial::create(glm::vec3{ 0.135f, 0.2225f, 0.1575f },             glm::vec3{ 0.54f, 0.89f, 0.63f },       glm::vec3{ 0.316228f, 0.316228f, 0.316228f }, 12.8f)},
            {      "Obsidian",       PureColorMaterial::create(glm::vec3{ 0.05375f, 0.05f, 0.06625f },       glm::vec3{ 0.18275f, 0.17f, 0.22525f },       glm::vec3{ 0.332741f, 0.328634f, 0.346435f }, 38.4f)},
            {         "Pearl",       PureColorMaterial::create(glm::vec3{ 0.25f, 0.20725f, 0.20725f },            glm::vec3{ 1.0f, 0.829f, 0.829f },       glm::vec3{ 0.296648f, 0.296648f, 0.296648f }, 12.7f)},
            {          "Ruby",     PureColorMaterial::create(glm::vec3{ 0.1745f, 0.01175f, 0.01175f },    glm::vec3{ 0.61424f, 0.04136f, 0.04136f },       glm::vec3{ 0.727811f, 0.626959f, 0.626959f }, 76.8f)},
            {     "Turquoise",         PureColorMaterial::create(glm::vec3{ 0.1f, 0.18725f, 0.1745f },      glm::vec3{ 0.396f, 0.74151f, 0.69102f },        glm::vec3{ 0.297254f, 0.30829f, 0.306678f }, 12.8f)},
            {         "Brass", PureColorMaterial::create(glm::vec3{ 0.329412f, 0.223529f, 0.027451f }, glm::vec3{ 0.780392f, 0.568627f, 0.113725f },       glm::vec3{ 0.992157f, 0.941176f, 0.807843f }, 27.9f)},
            {        "Bronze",        PureColorMaterial::create(glm::vec3{ 0.2125f, 0.1275f, 0.054f },       glm::vec3{ 0.714f, 0.4284f, 0.18144f },       glm::vec3{ 0.393548f, 0.271906f, 0.166721f }, 25.6f)},
            {        "Chrome",             PureColorMaterial::create(glm::vec3{ 0.25f, 0.25f, 0.25f },                glm::vec3{ 0.4f, 0.4f, 0.4f },       glm::vec3{ 0.774597f, 0.774597f, 0.774597f }, 76.8f)},
            {        "Copper",      PureColorMaterial::create(glm::vec3{ 0.19125f, 0.0735f, 0.0225f },      glm::vec3{ 0.7038f, 0.27048f, 0.0828f },       glm::vec3{ 0.256777f, 0.137622f, 0.086014f }, 12.8f)},
            {          "Gold",      PureColorMaterial::create(glm::vec3{ 0.24725f, 0.1995f, 0.0745f },    glm::vec3{ 0.75164f, 0.60648f, 0.22648f },       glm::vec3{ 0.628281f, 0.555802f, 0.366065f }, 51.2f)},
            {        "Silver",    PureColorMaterial::create(glm::vec3{ 0.19225f, 0.19225f, 0.19225f },    glm::vec3{ 0.50754f, 0.50754f, 0.50754f },       glm::vec3{ 0.508273f, 0.508273f, 0.508273f }, 51.2f)},
            { "Black plastic",                PureColorMaterial::create(glm::vec3{ 0.0f, 0.0f, 0.0f },             glm::vec3{ 0.01f, 0.01f, 0.01f },                   glm::vec3{ 0.50f, 0.50f, 0.50f }, 32.0f)},
            { "White plastic",                PureColorMaterial::create(glm::vec3{ 0.0f, 0.0f, 0.0f },             glm::vec3{ 0.55f, 0.55f, 0.55f },                   glm::vec3{ 0.70f, 0.70f, 0.70f }, 32.0f)},
            {   "Red plastic",                PureColorMaterial::create(glm::vec3{ 0.0f, 0.0f, 0.0f },                glm::vec3{ 0.5f, 0.0f, 0.0f },                      glm::vec3{ 0.7f, 0.6f, 0.6f }, 32.0f)},
            {"Yellow plastic",                PureColorMaterial::create(glm::vec3{ 0.0f, 0.0f, 0.0f },                glm::vec3{ 0.5f, 0.5f, 0.0f },                   glm::vec3{ 0.60f, 0.60f, 0.50f }, 32.0f)},
            { "Green plastic",                PureColorMaterial::create(glm::vec3{ 0.0f, 0.0f, 0.0f },               glm::vec3{ 0.1f, 0.35f, 0.1f },                   glm::vec3{ 0.45f, 0.55f, 0.45f }, 32.0f)},
            {  "Cyan plastic",               PureColorMaterial::create(glm::vec3{ 0.0f, 0.1f, 0.06f },  glm::vec3{ 0.0f, 0.50980392f, 0.50980392f }, glm::vec3{ 0.50196078f, 0.50196078f, 0.50196078f }, 32.0f)},
            {  "Black rubber",             PureColorMaterial::create(glm::vec3{ 0.02f, 0.02f, 0.02f },             glm::vec3{ 0.01f, 0.01f, 0.01f },                      glm::vec3{ 0.4f, 0.4f, 0.4f }, 10.0f)},
            {  "White rubber",             PureColorMaterial::create(glm::vec3{ 0.05f, 0.05f, 0.05f },                glm::vec3{ 0.5f, 0.5f, 0.5f },                      glm::vec3{ 0.7f, 0.7f, 0.7f }, 10.0f)},
            {    "Red rubber",               PureColorMaterial::create(glm::vec3{ 0.05f, 0.0f, 0.0f },                glm::vec3{ 0.5f, 0.4f, 0.4f },                    glm::vec3{ 0.7f, 0.04f, 0.04f }, 10.0f)},
            { "Yellow rubber",              PureColorMaterial::create(glm::vec3{ 0.05f, 0.05f, 0.0f },                glm::vec3{ 0.5f, 0.5f, 0.4f },                     glm::vec3{ 0.7f, 0.7f, 0.04f }, 10.0f)},
            {  "Green rubber",               PureColorMaterial::create(glm::vec3{ 0.0f, 0.05f, 0.0f },                glm::vec3{ 0.4f, 0.5f, 0.4f },                    glm::vec3{ 0.04f, 0.7f, 0.04f }, 10.0f)},
            {   "Cyan rubber",              PureColorMaterial::create(glm::vec3{ 0.0f, 0.05f, 0.05f },                glm::vec3{ 0.4f, 0.5f, 0.5f },                     glm::vec3{ 0.04f, 0.7f, 0.7f }, 10.0f)}
        };
        for (auto&& [name, material] : this->m_material) {
            material->set_shader(this->m_shader_library->get("PureColor"));
        }
        this->m_material.emplace_back("Texture", TextureMaterial::create(dusk::Texture2D::create(dusk::get_file_path("assets/images/container2.png")), dusk::Texture2D::create(dusk::get_file_path("assets/images/container2_specular.png")), 64.0f, this->m_shader_library->get("Texture")));

        this->m_light.emplace_back("Directional", DirectionalLight::create(glm::vec3{ 1.2f, 1.0f, 2.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f }, glm::vec3{ 0.2f, 0.2f, 0.2f }, glm::vec3{ 0.5f, 0.5f, 0.5f }, glm::vec3{ 1.0f, 1.0f, 1.0f }));
        this->m_light.emplace_back("Point", PointLight::create(glm::vec3{ 1.2f, 1.0f, 2.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f }, glm::vec3{ 0.2f, 0.2f, 0.2f }, glm::vec3{ 0.5f, 0.5f, 0.5f }, glm::vec3{ 1.0f, 1.0f, 1.0f }));
    }
    // Framebuffer.
    {
        auto window{ dusk::Application::get()->get_window() };
        dusk::FramebufferProps props{
            {window->get_width(), window->get_height()}
        };
        this->m_frame_buffer = dusk::Framebuffer::create(props);
    }
}

void LightLayer::on_detach() {}

void LightLayer::on_ImGui_render() {
    ImGuiViewport const* main_viewport{ ImGui::GetMainViewport() };

    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 30, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver);

    ImGui::Begin("Test for ImGui.");

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::ColorEdit4("BG color", glm::value_ptr(this->m_bg_color));

    static std::size_t camera_type_idx{ (std::size_t)dusk::CameraType::Perspective };
    static std::array<char const*, 2> camera_type{ "Orthographic Camera", "Perspective Camera" };
    if (ImGui::BeginCombo("Camera Type", camera_type[camera_type_idx])) {
        for (std::size_t i{ 0 }; i < camera_type.size(); ++i) {
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

    if (ImGui::BeginCombo("Material Option", this->m_material[this->m_material_idx].first.c_str())) {
        for (std::size_t i{ 0 }; i < this->m_material.size(); ++i) {
            bool is_selected{ (this->m_material_idx == i) };
            if (ImGui::Selectable(this->m_material[i].first.c_str(), is_selected)) {
                this->m_material_idx = i;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo("Light Option", this->m_light[this->m_light_idx].first.c_str())) {
        for (std::size_t i{ 0 }; i < this->m_light.size(); ++i) {
            bool is_selected{ (this->m_light_idx == i) };
            if (ImGui::Selectable(this->m_light[i].first.c_str(), is_selected)) {
                this->m_light_idx = i;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::ColorEdit3("Light Intensity", glm::value_ptr(this->m_light[this->m_light_idx].second->color));
    if (auto dl{ std::dynamic_pointer_cast<DirectionalLight>(this->m_light[this->m_light_idx].second) }) {
        ImGui::DragFloat3("Light Direction", glm::value_ptr(dl->direction), 0.05f);
    }
    else if (auto pl{ std::dynamic_pointer_cast<PointLight>(this->m_light[this->m_light_idx].second) }) {
        ImGui::DragFloat3("Light Position", glm::value_ptr(pl->position), 0.05f);
    }

    ImGui::End();
}

void LightLayer::on_update() {
    dusk::RenderCommand::clear();
    dusk::RenderCommand::set_clear_color(this->m_bg_color);

    this->m_trackball->on_update();

    dusk::Renderer::begin_scene(this->m_trackball->get_camera());

    auto m{ this->m_material[this->m_material_idx].second };
    m->bind("u_Material");
    auto l{ this->m_light[this->m_light_idx].second };
    l->set_shader(m->get_shader());
    l->bind("u_Light");
    dusk::Renderer::submit(m->get_shader().get(), this->m_cube_vao.get(), glm::mat4(1.0f));

    auto model{ glm::scale(glm::mat4(1.0f), glm::vec3(0.2f)) };
    if (auto dl{ std::dynamic_pointer_cast<DirectionalLight>(l) }) {
        model = glm::translate(glm::mat4(1.0f), dl->direction) * model;
    }
    else if (auto pl{ std::dynamic_pointer_cast<PointLight>(l) }) {
        model = glm::translate(glm::mat4(1.0f), pl->position) * model;
    }
    auto light_shader{ this->m_shader_library->get("Light") };
    light_shader->bind();
    light_shader->set_vec3("u_LightColor", l->color);
    dusk::Renderer::submit(light_shader.get(), this->m_light_cube_vao.get(), model);

    dusk::Renderer::end_scene();
}

void LightLayer::on_event(dusk::EventBase& e) {
    this->m_trackball->on_event(e);
}

LEARN_OPENGL_NAMESPACE_END
