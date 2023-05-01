#ifndef _LEARN_OPENGL_ADVANCED_LAYER_HPP_
#define _LEARN_OPENGL_ADVANCED_LAYER_HPP_

#include <dusk.hpp>
#include <material/material.hpp>
#include <light/light.hpp>
#include <common.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

class AdvancedLayer : public dusk::Layer {
private:
    std::unique_ptr<dusk::ShaderLibrary> m_shader_library;
    std::unordered_map<std::string, std::unique_ptr<dusk::VertexArray>> m_vao;
    std::unordered_map<std::string, std::shared_ptr<dusk::Texture>> m_texture;
    std::unique_ptr<dusk::TrackBall> m_trackball;
    std::unique_ptr<dusk::Framebuffer> m_frame_buffer;

    glm::vec4 m_bg_color{ 0.1f, 0.1f, 0.1f, 1.0f };
    std::size_t m_shader_idx{ 0 };
    std::vector<glm::vec3> vegetation;

public:
    AdvancedLayer();
    virtual ~AdvancedLayer() = default;

    virtual void on_attach() override;
    virtual void on_detach() override;
    virtual void on_ImGui_render() override;
    virtual void on_update() override;
    virtual void on_event(dusk::EventBase& e) override;
};

LEARN_OPENGL_NAMESPACE_END

#endif // !_LEARN_OPENGL_ADVANCED_LAYER_HPP_
