#include <material/pure-color-material.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

PureColorMaterial::PureColorMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, std::shared_ptr<dusk::Shader> const& shader)
    : Material{ shader }
    , ambient{ ambient }
    , diffuse{ diffuse }
    , specular{ specular }
    , shininess{ shininess } {}

void PureColorMaterial::bind(std::string const& base_name) const {
    this->shader->bind();
    this->shader->set_vec3(base_name + ".ambient", this->ambient);
    this->shader->set_vec3(base_name + ".diffuse", this->diffuse);
    this->shader->set_vec3(base_name + ".specular", this->specular);
    this->shader->set_float(base_name + ".shininess", this->shininess);
}

LEARN_OPENGL_NAMESPACE_END
