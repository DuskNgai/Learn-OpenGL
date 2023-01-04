#ifndef _LEARN_OPENGL_MATERIAL_BASE_MATERIAL_HPP_
#define _LEARN_OPENGL_MATERIAL_BASE_MATERIAL_HPP_

#include <Dusk.hpp>
#include <Common.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

/// @brief The base class for materials.
class Material {
protected:
    std::shared_ptr<Dusk::Shader> shader;

public:
    Material(std::shared_ptr<Dusk::Shader> shader = nullptr);
    virtual ~Material() = default;

public:
    /// @brief Set the shader for the material.
    void SetShader(std::shared_ptr<Dusk::Shader> shader);
    /// @brief Get the shader of the material.
    std::shared_ptr<Dusk::Shader> GetShader() const;

    /// @brief Bind the material to the shader.
    virtual void Bind(std::string const& base_name) const = 0;
};

LEARN_OPENGL_NAMESPACE_END

#endif // !_LEARN_OPENGL_MATERIAL_BASE_MATERIAL_HPP_
