#ifndef _LEARN_OPENGL_MATERIAL_PURE_COLOR_MATERIAL_HPP_
#define _LEARN_OPENGL_MATERIAL_PURE_COLOR_MATERIAL_HPP_

#include <material/base-material.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

/// @brief The material only has pure color.
class PureColorMaterial : public Material {
public:
    glm::vec3 ambient{ 0.0f, 0.0f, 0.0f };
    glm::vec3 diffuse{ 0.0f, 0.0f, 0.0f };
    glm::vec3 specular{ 0.0f, 0.0f, 0.0f };
    float shininess{ 0.0f };

public:
    PureColorMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, std::shared_ptr<dusk::Shader> const& shader = nullptr);

public:
    template <typename... Args>
    static std::shared_ptr<PureColorMaterial> create(Args&&... args) {
        return std::make_shared<PureColorMaterial>(std::forward<Args>(args)...);
    }

    virtual void bind(std::string const& base_name) const override;
};

LEARN_OPENGL_NAMESPACE_END

#endif // !_LEARN_OPENGL_MATERIAL_PURE_COLOR_MATERIAL_HPP_
