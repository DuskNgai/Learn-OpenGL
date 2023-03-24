#include <material/base-material.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

Material::Material(std::shared_ptr<dusk::Shader> shader)
    : shader(shader) {}

void Material::set_shader(std::shared_ptr<dusk::Shader> shader) { this->shader = shader; }

std::shared_ptr<dusk::Shader> Material::get_shader() const { return this->shader; }

LEARN_OPENGL_NAMESPACE_END
