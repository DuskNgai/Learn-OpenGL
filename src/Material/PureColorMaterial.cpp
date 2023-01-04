#include <Material/PureColorMaterial.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

PureColorMaterial::PureColorMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
    : PureColorMaterial(nullptr, ambient, diffuse, specular, shininess) {}

PureColorMaterial::PureColorMaterial(std::shared_ptr<Dusk::Shader> shader, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
    : Material(shader)
    , ambient(ambient)
    , diffuse(diffuse)
    , specular(specular)
    , shininess(shininess) {}

void PureColorMaterial::Bind(std::string const& base_name) const {
    shader->Bind();
    shader->SetVec3(base_name + ".ambient", this->ambient);
    shader->SetVec3(base_name + ".diffuse", this->diffuse);
    shader->SetVec3(base_name + ".specular", this->specular);
    shader->SetFloat(base_name + ".shininess", this->shininess);
}

LEARN_OPENGL_NAMESPACE_END
