#include <Light/PointLight.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

PointLight::PointLight(glm::vec3 position, glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : PointLight(position, color, ambient, diffuse, specular, nullptr) {}

PointLight::PointLight(glm::vec3 position, glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, std::shared_ptr<Dusk::Shader> shader)
    : Light(color, ambient, diffuse, specular, shader)
    , position(position) {}

std::shared_ptr<PointLight> PointLight::Create(glm::vec3 position, glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
    return std::make_shared<PointLight>(position, color, ambient, diffuse, specular);
}

std::shared_ptr<PointLight> PointLight::Create(glm::vec3 position, glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, std::shared_ptr<Dusk::Shader> shader) {
    return std::make_shared<PointLight>(position, color, ambient, diffuse, specular, shader);
}

void PointLight::Bind(std::string const& base_name) const {
    Light::Bind(base_name);
    this->shader->SetVec3(base_name + ".position", this->position);
    this->shader->SetInt(base_name + ".pos_or_dir", 0);
}

LEARN_OPENGL_NAMESPACE_END
