#ifndef _LEARN_OPENGL_EXAMPLE_LAYER_HPP_
#define _LEARN_OPENGL_EXAMPLE_LAYER_HPP_

#include <Dusk.hpp>
#include <Common.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

class ExampleLayer : public Dusk::Layer {
private:
    std::unique_ptr<Dusk::ShaderLibrary> m_shader_library;

    std::unique_ptr<Dusk::VertexArray> m_vertex_array;
    std::unique_ptr<Dusk::VertexArray> m_texture_vertex_array;

    std::shared_ptr<Dusk::Texture> m_texture;
    std::shared_ptr<Dusk::Texture> m_alpha_texture;

    std::shared_ptr<Dusk::FrameBuffer> m_frame_buffer;

    std::unique_ptr<Dusk::TrackBall> m_trackball;

    glm::vec4 m_bg_color{0.1f, 0.1f, 0.1f, 1.0f};

public:
    ExampleLayer();
    virtual ~ExampleLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnImGuiRender() override;
    virtual void OnUpdate() override;
    virtual void OnEvent(Dusk::EventBase& e) override;
};

LEARN_OPENGL_NAMESPACE_END

#endif // !_LEARN_OPENGL_EXAMPLE_LAYER_HPP_
