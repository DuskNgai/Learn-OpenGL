#ifndef _LEARN_OPENGL_LIGHT_LAYER_HPP_
#define _LEARN_OPENGL_LIGHT_LAYER_HPP_

#include <Dusk.hpp>
#include <Material/Material.hpp>
#include <Light.hpp>
#include <Common.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

class LightLayer : public Dusk::Layer {
private:
    std::unique_ptr<Dusk::VertexArray> m_cube_vao;
    std::unique_ptr<Dusk::VertexArray> m_light_cube_vao;
    std::unique_ptr<Dusk::TrackBall> m_trackball;
    std::unique_ptr<Dusk::ShaderLibrary> m_shader_library;
    std::shared_ptr<Dusk::FrameBuffer> m_frame_buffer;

    int m_material_idx{0};
    std::vector<std::pair<std::string, std::shared_ptr<Material>>> m_material;
    std::shared_ptr<Light> m_light;

public:
    LightLayer();
    virtual ~LightLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnImGuiRender() override;
    virtual void OnUpdate() override;
    virtual void OnEvent(Dusk::EventBase& e) override;
};

LEARN_OPENGL_NAMESPACE_END

#endif // !_LEARN_OPENGL_LIGHT_LAYER_HPP_
