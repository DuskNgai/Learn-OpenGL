#ifndef _LEARN_OPENGL_LIGHT_POINT_LIGHT_HPP_
#define _LEARN_OPENGL_LIGHT_POINT_LIGHT_HPP_

#include <light/base-light.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

class PointLight : public Light {
public:
    glm::vec3 position{ 0.0f, 0.0f, 0.0f };

public:
    PointLight(glm::vec3 position, glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, std::shared_ptr<dusk::Shader> const& shader = nullptr);

public:
    template <typename... Args>
    static std::shared_ptr<PointLight> create(Args&&... args) {
        return std::make_shared<PointLight>(std::forward<Args>(args)...);
    }

    virtual void bind(std::string const& base_name) const override;
};

LEARN_OPENGL_NAMESPACE_END

#endif // !_LEARN_OPENGL_LIGHT_POINT_LIGHT_HPP_
