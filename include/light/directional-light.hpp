#ifndef _LEARN_OPENGL_LIGHT_DIRECTIONAL_LIGHT_HPP_
#define _LEARN_OPENGL_LIGHT_DIRECTIONAL_LIGHT_HPP_

#include <light/base-light.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

class DirectionalLight : public Light {
public:
    glm::vec3 direction{ 0.0f, 0.0f, 0.0f };

public:
    DirectionalLight(glm::vec3 direction, glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, std::shared_ptr<dusk::Shader> shader = nullptr);

public:
    template <typename... Args>
    static std::shared_ptr<DirectionalLight> create(Args&&... args) {
        return std::make_shared<DirectionalLight>(std::forward<Args>(args)...);
    }

    virtual void bind(std::string const& base_name) const override;
};

LEARN_OPENGL_NAMESPACE_END

#endif // !_LEARN_OPENGL_LIGHT_DIRECTIONAL_LIGHT_HPP_
