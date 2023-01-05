#ifndef _LEARN_OPENGL_LIGHT_BASE_LIGHT_HPP_
#define _LEARN_OPENGL_LIGHT_BASE_LIGHT_HPP_

#include <Dusk.hpp>
#include <Common.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

/// @brief The base class for lights.
class Light {
public:
    glm::vec3 color{0.0f, 0.0f, 0.0f};
    glm::vec3 ambient{0.0f, 0.0f, 0.0f};
    glm::vec3 diffuse{0.0f, 0.0f, 0.0f};
    glm::vec3 specular{0.0f, 0.0f, 0.0f};

protected:
    std::shared_ptr<Dusk::Shader> shader;

public:
    Light(glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, std::shared_ptr<Dusk::Shader> shader = nullptr);
    virtual ~Light() = default;

public:
    /// @brief Set the shader for the light.
    void SetShader(std::shared_ptr<Dusk::Shader> shader);
    /// @brief Get the shader of the light.
    std::shared_ptr<Dusk::Shader> GetShader() const;

    /// @brief Bind the light to the shader.
    virtual void Bind(std::string const& base_name) const;
};

LEARN_OPENGL_NAMESPACE_END

#endif // !_LEARN_OPENGL_LIGHT_BASE_LIGHT_HPP_
