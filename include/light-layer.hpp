#ifndef _LEARN_OPENGL_LIGHT_LAYER_HPP_
#define _LEARN_OPENGL_LIGHT_LAYER_HPP_

#include <dusk.hpp>
#include <material/material.hpp>
#include <light/light.hpp>
#include <common.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

class LightLayer : public dusk::Layer {
private:
    std::unique_ptr<dusk::ShaderLibrary> m_shader_library;
    std::unordered_map<std::string, std::unique_ptr<dusk::VertexArray>> m_vao;
    std::unique_ptr<dusk::TrackBall> m_trackball;

    glm::vec4 m_bg_color{ 0.1f, 0.1f, 0.1f, 1.0f };
    dusk::IndexedMap<std::string, std::shared_ptr<Material>> m_material;
    dusk::IndexedMap<std::string, std::shared_ptr<Light>> m_light;

public:
    LightLayer();
    virtual ~LightLayer() = default;

    virtual void on_attach() override;
    virtual void on_detach() override;
    virtual void on_ImGui_render() override;
    virtual void on_update() override;
    virtual void on_event(dusk::EventBase& e) override;
};

LEARN_OPENGL_NAMESPACE_END

#endif // !_LEARN_OPENGL_LIGHT_LAYER_HPP_
