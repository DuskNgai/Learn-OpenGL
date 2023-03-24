#include <light/directional-light.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, std::shared_ptr<dusk::Shader> shader)
    : Light{ color, ambient, diffuse, specular, shader }
    , direction{ direction } {}

void DirectionalLight::bind(std::string const& base_name) const {
    Light::bind(base_name);
    this->shader->set_vec3(base_name + ".direction", this->direction);
    this->shader->set_int(base_name + ".pos_or_dir", 1);
}

LEARN_OPENGL_NAMESPACE_END
