#include <light/point-light.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

PointLight::PointLight(glm::vec3 position, glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, std::shared_ptr<dusk::Shader> shader)
    : Light{ color, ambient, diffuse, specular, shader }
    , position{ position } {}

void PointLight::bind(std::string const& base_name) const {
    Light::bind(base_name);
    this->shader->set_vec3(base_name + ".position", this->position);
    this->shader->set_int(base_name + ".pos_or_dir", 0);
}

LEARN_OPENGL_NAMESPACE_END
