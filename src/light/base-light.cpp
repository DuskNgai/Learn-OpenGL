#include <light/base-light.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

Light::Light(glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, std::shared_ptr<dusk::Shader> shader)
    : color{ color }
    , ambient{ ambient }
    , diffuse{ diffuse }
    , specular{ specular }
    , shader{ shader } {}

void Light::set_shader(std::shared_ptr<dusk::Shader> shader) { this->shader = shader; }

std::shared_ptr<dusk::Shader> Light::get_shader() const { return this->shader; }

void Light::bind(std::string const& base_name) const {
    this->shader->bind();
    this->shader->set_vec3(base_name + ".ambient", this->ambient * this->color);
    this->shader->set_vec3(base_name + ".diffuse", this->diffuse * this->color);
    this->shader->set_vec3(base_name + ".specular", this->specular * this->color);
}

LEARN_OPENGL_NAMESPACE_END
