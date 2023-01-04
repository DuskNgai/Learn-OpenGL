#include <Material/TextureMaterial.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

TextureMaterial::TextureMaterial(std::shared_ptr<Dusk::Texture2D> diffuse, std::shared_ptr<Dusk::Texture2D> specular, float shininess)
    : TextureMaterial(nullptr, diffuse, specular, shininess) {}

TextureMaterial::TextureMaterial(std::shared_ptr<Dusk::Shader> shader, std::shared_ptr<Dusk::Texture2D> diffuse, std::shared_ptr<Dusk::Texture2D> specular, float shininess)
    : Material(shader)
    , diffuse(diffuse)
    , specular(specular)
    , shininess(shininess) {}

void TextureMaterial::Bind(std::string const& base_name) const {
    shader->Bind();
    this->diffuse->Bind(0);
    shader->SetInt(base_name + ".diffuse", 0);
    this->specular->Bind(1);
    shader->SetInt(base_name + ".specular", 1);
    shader->SetFloat(base_name + ".shininess", this->shininess);
}

LEARN_OPENGL_NAMESPACE_END
