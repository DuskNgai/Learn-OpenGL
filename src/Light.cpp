#include <Light.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

Light::Light(glm::vec3 position, glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : Light(nullptr, position, color, ambient, diffuse, specular) {}

Light::Light(std::shared_ptr<Dusk::Shader> shader, glm::vec3 position, glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : shader(shader)
    , position(position)
    , color(color)
    , ambient(ambient)
    , diffuse(diffuse)
    , specular(specular) {}

void Light::SetShader(std::shared_ptr<Dusk::Shader> shader) { this->shader = shader; }

std::shared_ptr<Dusk::Shader> Light::GetShader() const { return this->shader; }

void Light::Bind(std::string const& base_name) {
    shader->Bind();
    shader->SetVec3(base_name + ".position", this->position * this->color);
    shader->SetVec3(base_name + ".ambient", this->ambient * this->color);
    shader->SetVec3(base_name + ".diffuse", this->diffuse * this->color);
    shader->SetVec3(base_name + ".specular", this->specular * this->color);
}

LEARN_OPENGL_NAMESPACE_END
