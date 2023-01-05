#include <Light/BaseLight.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

Light::Light(glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, std::shared_ptr<Dusk::Shader> shader)
    :  color(color)
    , ambient(ambient)
    , diffuse(diffuse)
    , specular(specular)
    , shader(shader) {}

void Light::SetShader(std::shared_ptr<Dusk::Shader> shader) { this->shader = shader; }

std::shared_ptr<Dusk::Shader> Light::GetShader() const { return this->shader; }

void Light::Bind(std::string const& base_name) const {
    this->shader->Bind();
    this->shader->SetVec3(base_name + ".ambient", this->ambient * this->color);
    this->shader->SetVec3(base_name + ".diffuse", this->diffuse * this->color);
    this->shader->SetVec3(base_name + ".specular", this->specular * this->color);
}

LEARN_OPENGL_NAMESPACE_END
