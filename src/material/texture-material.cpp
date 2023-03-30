#include <material/texture-material.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

TextureMaterial::TextureMaterial(std::shared_ptr<dusk::Texture2D> diffuse, std::shared_ptr<dusk::Texture2D> specular, float shininess, std::shared_ptr<dusk::Shader> const& shader)
    : Material{ shader }
    , diffuse{ diffuse }
    , specular{ specular }
    , shininess{ shininess } {}

void TextureMaterial::bind(std::string const& base_name) const {
    this->shader->bind();
    this->diffuse->bind(0);
    this->shader->set_int(base_name + ".diffuse", 0);
    this->specular->bind(1);
    this->shader->set_int(base_name + ".specular", 1);
    this->shader->set_float(base_name + ".shininess", this->shininess);
}

LEARN_OPENGL_NAMESPACE_END
