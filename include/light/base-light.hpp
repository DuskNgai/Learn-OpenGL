#ifndef _LEARN_OPENGL_LIGHT_BASE_LIGHT_HPP_
#define _LEARN_OPENGL_LIGHT_BASE_LIGHT_HPP_

#include <dusk.hpp>
#include <common.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

/// @brief The base class for lights.
class Light {
public:
    glm::vec3 color{ 0.0f, 0.0f, 0.0f };
    glm::vec3 ambient{ 0.0f, 0.0f, 0.0f };
    glm::vec3 diffuse{ 0.0f, 0.0f, 0.0f };
    glm::vec3 specular{ 0.0f, 0.0f, 0.0f };

protected:
    std::shared_ptr<dusk::Shader> shader;

public:
    Light(glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, std::shared_ptr<dusk::Shader> const& shader = nullptr);
    virtual ~Light() = default;

public:
    void set_shader(std::shared_ptr<dusk::Shader> const& shader);
    std::shared_ptr<dusk::Shader> get_shader() const;

    /// @brief Bind the light to the shader.
    virtual void bind(std::string const& base_name) const;
};

LEARN_OPENGL_NAMESPACE_END

#endif // !_LEARN_OPENGL_LIGHT_BASE_LIGHT_HPP_
