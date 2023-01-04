#include <Material/BaseMaterial.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

Material::Material(std::shared_ptr<Dusk::Shader> shader) : shader(shader) {}

void Material::SetShader(std::shared_ptr<Dusk::Shader> shader) { this->shader = shader; }

std::shared_ptr<Dusk::Shader> Material::GetShader() const { return this->shader; }

LEARN_OPENGL_NAMESPACE_END
