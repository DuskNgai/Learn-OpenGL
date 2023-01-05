#include <Light/DirectionalLight.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : DirectionalLight(direction, color, ambient, diffuse, specular, nullptr) {}

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, std::shared_ptr<Dusk::Shader> shader)
    : Light(color, ambient, diffuse, specular, shader)
    , direction(direction) {}

std::shared_ptr<DirectionalLight> DirectionalLight::Create(glm::vec3 direction, glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
    return std::make_shared<DirectionalLight>(direction, color, ambient, diffuse, specular);
}

std::shared_ptr<DirectionalLight> DirectionalLight::Create(glm::vec3 direction, glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, std::shared_ptr<Dusk::Shader> shader) {
    return std::make_shared<DirectionalLight>(direction, color, ambient, diffuse, specular, shader);
}

void DirectionalLight::Bind(std::string const& base_name) const {
    Light::Bind(base_name);
    this->shader->SetVec3(base_name + ".direction", this->direction);
    this->shader->SetInt(base_name + ".pos_or_dir", 1);
}

LEARN_OPENGL_NAMESPACE_END
