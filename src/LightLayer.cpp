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
            "PureColor",
            Dusk::Shader::Create(
                Dusk::ReadTextFile(Dusk::GetFilePath("assets/shaders/lighting/ObjectVertexShader.glsl")),
                Dusk::ReadTextFile(Dusk::GetFilePath("assets/shaders/lighting/PureColorFragmentShader.glsl"))
            )
        );
        this->m_shader_library->Add(
            "Texture",
            Dusk::Shader::Create(
                Dusk::ReadTextFile(Dusk::GetFilePath("assets/shaders/lighting/ObjectVertexShader.glsl")),
                Dusk::ReadTextFile(Dusk::GetFilePath("assets/shaders/lighting/TextureFragmentShader.glsl"))
            )
        );
        this->m_shader_library->Add(
            "Light",
            Dusk::Shader::Create(
                Dusk::ReadTextFile(Dusk::GetFilePath("assets/shaders/lighting/ObjectVertexShader.glsl")),
                Dusk::ReadTextFile(Dusk::GetFilePath("assets/shaders/lighting/LightFragmentShader.glsl"))
            )
        );
    }
    // Texture & Material & Light.
    {
        this->m_material = s_named_pure_color_materials;
        for (auto [name, material]: this->m_material) {
            material->SetShader(this->m_shader_library->Get("PureColor"));
        }
        this->m_material.emplace_back("Texture", TextureMaterial::Create(
            Dusk::Texture2D::Create(Dusk::GetFilePath("assets/images/container2.png")),
            Dusk::Texture2D::Create(Dusk::GetFilePath("assets/images/container2_specular.png")),
            64.0f,
            this->m_shader_library->Get("Texture")
        ));

        this->m_light.emplace_back("Directional", DirectionalLight::Create(
            {1.2f, 1.0f, 2.0f}, {1.0f, 1.0f, 1.0f}, {0.2f, 0.2f, 0.2f}, {0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}
        ));
        this->m_light.emplace_back("Point", PointLight::Create(
            {1.2f, 1.0f, 2.0f}, {1.0f, 1.0f, 1.0f}, {0.2f, 0.2f, 0.2f}, {0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}
        ));
    }
    // Framebuffer.
    {
        Dusk::FrameBufferProps props{1280, 720};
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

    static std::size_t camera_type_idx = (std::size_t)Dusk::CameraType::Perspective;
    static std::array<char const*, 2> camera_type{"Orthographic Camera", "Perspective Camera"};
    if (ImGui::BeginCombo("Camera Type", camera_type[camera_type_idx])) {
        for (std::size_t i = 0; i < camera_type.size(); ++i) {
            bool is_selected = (camera_type_idx == i);
            if (ImGui::Selectable(camera_type[i], is_selected)) {
                camera_type_idx = i;
                this->m_trackball->SetCameraType((Dusk::CameraType)camera_type_idx);
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    if (ImGui::BeginCombo("Material Option", this->m_material[this->m_material_idx].first.c_str())) {
        for (std::size_t i = 0; i < this->m_material.size(); ++i) {
            bool is_selected = (this->m_material_idx == i);
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
        for (std::size_t i = 0; i < this->m_light.size(); ++i) {
            bool is_selected = (this->m_light_idx == i);
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
    if (auto dl = std::dynamic_pointer_cast<DirectionalLight>(this->m_light[this->m_light_idx].second)) {
        ImGui::DragFloat3("Light Direction", glm::value_ptr(dl->direction), 0.05f);
    } else if (auto pl = std::dynamic_pointer_cast<PointLight>(this->m_light[this->m_light_idx].second)) {
        ImGui::DragFloat3("Light Position", glm::value_ptr(pl->position), 0.05f);
    }

    ImGui::End();
}

void LightLayer::OnUpdate() {
    Dusk::RenderCommand::Clear();
    Dusk::RenderCommand::SetClearColor(this->m_bg_color);

    this->m_trackball->OnUpdate();

    Dusk::Renderer::BeginScene(this->m_trackball->GetCamera());

    auto m = this->m_material[this->m_material_idx].second;
    m->Bind("u_Material");
    auto l = this->m_light[this->m_light_idx].second;
    l->SetShader(m->GetShader());
    l->Bind("u_Light");
    Dusk::Renderer::Submit(m->GetShader().get(), this->m_cube_vao.get(), glm::mat4(1.0f));

    auto model = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
    if (auto dl = std::dynamic_pointer_cast<DirectionalLight>(l)) {
        model = glm::translate(glm::mat4(1.0f), dl->direction) * model;
    } else if (auto pl = std::dynamic_pointer_cast<PointLight>(l)) {
        model = glm::translate(glm::mat4(1.0f), pl->position) * model;
    }
    auto light_shader = this->m_shader_library->Get("Light");
    light_shader->Bind();
    light_shader->SetVec3("u_LightColor", l->color);
    Dusk::Renderer::Submit(light_shader.get(), this->m_light_cube_vao.get(), model);

    Dusk::Renderer::EndScene();
}

void LightLayer::OnEvent(Dusk::EventBase& e) {
    this->m_trackball->OnEvent(e);
}

LEARN_OPENGL_NAMESPACE_END
