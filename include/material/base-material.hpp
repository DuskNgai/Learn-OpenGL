#ifndef _LEARN_OPENGL_MATERIAL_BASE_MATERIAL_HPP_
#define _LEARN_OPENGL_MATERIAL_BASE_MATERIAL_HPP_

#include <dusk.hpp>
#include <common.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

/// @brief The base class for materials.
class Material {
protected:
    std::shared_ptr<dusk::Shader> shader;

public:
    Material(std::shared_ptr<dusk::Shader> const& shader = nullptr);
    virtual ~Material() = default;

public:
    void set_shader(std::shared_ptr<dusk::Shader> const& shader);
    std::shared_ptr<dusk::Shader> get_shader() const;

    /// @brief Bind the material to the shader.
    virtual void bind(std::string const& base_name) const = 0;
};

LEARN_OPENGL_NAMESPACE_END

#endif // !_LEARN_OPENGL_MATERIAL_BASE_MATERIAL_HPP_
