#include <Material/TextureMaterial.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

TextureMaterial::TextureMaterial(std::shared_ptr<Dusk::Texture2D> diffuse, std::shared_ptr<Dusk::Texture2D> specular, float shininess)
    : TextureMaterial(diffuse, specular, shininess, nullptr) {}

TextureMaterial::TextureMaterial(std::shared_ptr<Dusk::Texture2D> diffuse, std::shared_ptr<Dusk::Texture2D> specular, float shininess, std::shared_ptr<Dusk::Shader> shader)
    : Material(shader)
    , diffuse(diffuse)
    , specular(specular)
    , shininess(shininess) {}

std::shared_ptr<TextureMaterial> TextureMaterial::Create(std::shared_ptr<Dusk::Texture2D> diffuse, std::shared_ptr<Dusk::Texture2D> specular, float shininess) {
    return std::make_shared<TextureMaterial>(diffuse, specular, shininess);
}

std::shared_ptr<TextureMaterial> TextureMaterial::Create(std::shared_ptr<Dusk::Texture2D> diffuse, std::shared_ptr<Dusk::Texture2D> specular, float shininess, std::shared_ptr<Dusk::Shader> shader) {
    return std::make_shared<TextureMaterial>(diffuse, specular, shininess, shader);
}

void TextureMaterial::Bind(std::string const& base_name) const {
    this->shader->Bind();
    this->diffuse->Bind(0);
    this->shader->SetInt(base_name + ".diffuse", 0);
    this->specular->Bind(1);
    this->shader->SetInt(base_name + ".specular", 1);
    this->shader->SetFloat(base_name + ".shininess", this->shininess);
}

LEARN_OPENGL_NAMESPACE_END
