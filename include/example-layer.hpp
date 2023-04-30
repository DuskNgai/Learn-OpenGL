#ifndef _LEARN_OPENGL_EXAMPLE_LAYER_HPP_
#define _LEARN_OPENGL_EXAMPLE_LAYER_HPP_

#include <dusk.hpp>
#include <common.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

class ExampleLayer : public dusk::Layer {
private:
    std::unique_ptr<dusk::ShaderLibrary> m_shader_library;

    std::unordered_map<std::string, std::unique_ptr<dusk::VertexArray>> m_vertex_array;

    std::unordered_map<std::string, std::shared_ptr<dusk::Texture>> m_texture;

    std::unique_ptr<dusk::Framebuffer> m_frame_buffer;

    std::unique_ptr<dusk::TrackBall> m_trackball;

    glm::vec2 m_viewport_size{ 0.0f, 0.0f };
    glm::vec4 m_bg_color{ 0.1f, 0.1f, 0.1f, 1.0f };

    bool m_is_viewport_focused{ false };
    bool m_is_viewport_hovered{ false };

public:
    ExampleLayer();
    virtual ~ExampleLayer() = default;

    virtual void on_attach() override;
    virtual void on_detach() override;
    virtual void on_ImGui_render() override;
    virtual void on_update() override;
    virtual void on_event(dusk::EventBase& e) override;
};

LEARN_OPENGL_NAMESPACE_END

#endif // !_LEARN_OPENGL_EXAMPLE_LAYER_HPP_
