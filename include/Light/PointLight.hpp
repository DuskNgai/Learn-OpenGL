#ifndef _LEARN_OPENGL_LIGHT_POINT_LIGHT_HPP_
#define _LEARN_OPENGL_LIGHT_POINT_LIGHT_HPP_

#include <Light/BaseLight.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

class PointLight : public Light {
public:
    glm::vec3 position{0.0f, 0.0f, 0.0f};

public:
    PointLight(glm::vec3 position, glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    PointLight(glm::vec3 position, glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, std::shared_ptr<Dusk::Shader> shader);

public:
    static std::shared_ptr<PointLight> Create(glm::vec3 position, glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    static std::shared_ptr<PointLight> Create(glm::vec3 position, glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, std::shared_ptr<Dusk::Shader> shader);

    virtual void Bind(std::string const& base_name) const override;
};

LEARN_OPENGL_NAMESPACE_END

#endif // !_LEARN_OPENGL_LIGHT_POINT_LIGHT_HPP_
