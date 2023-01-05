#include <Material/PureColorMaterial.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

PureColorMaterial::PureColorMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
    : PureColorMaterial(ambient, diffuse, specular, shininess, nullptr) {}

PureColorMaterial::PureColorMaterial( glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, std::shared_ptr<Dusk::Shader> shader)
    : Material(shader)
    , ambient(ambient)
    , diffuse(diffuse)
    , specular(specular)
    , shininess(shininess) {}

std::shared_ptr<PureColorMaterial> PureColorMaterial::Create(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) {
    return std::make_shared<PureColorMaterial>(ambient, diffuse, specular, shininess);
}

std::shared_ptr<PureColorMaterial> PureColorMaterial::Create(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, std::shared_ptr<Dusk::Shader> shader) {
    return std::make_shared<PureColorMaterial>(ambient, diffuse, specular, shininess, shader);
}

void PureColorMaterial::Bind(std::string const& base_name) const {
    this->shader->Bind();
    this->shader->SetVec3(base_name + ".ambient", this->ambient);
    this->shader->SetVec3(base_name + ".diffuse", this->diffuse);
    this->shader->SetVec3(base_name + ".specular", this->specular);
    this->shader->SetFloat(base_name + ".shininess", this->shininess);
}

LEARN_OPENGL_NAMESPACE_END
