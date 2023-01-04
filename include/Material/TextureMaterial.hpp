#ifndef _LEARN_OPENGL_MATERIAL_TEXTURE_MATERIAL_HPP_
#define _LEARN_OPENGL_MATERIAL_TEXTURE_MATERIAL_HPP_

#include <Material/BaseMaterial.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

/// @brief The material that need textures.
class TextureMaterial : public Material {
public:
    std::shared_ptr<Dusk::Texture2D> diffuse;
    std::shared_ptr<Dusk::Texture2D> specular;
    float shininess{0.0f};

public:
    TextureMaterial(std::shared_ptr<Dusk::Texture2D> diffuse, std::shared_ptr<Dusk::Texture2D> specular, float shininess);
    TextureMaterial(std::shared_ptr<Dusk::Shader> shader, std::shared_ptr<Dusk::Texture2D> diffuse, std::shared_ptr<Dusk::Texture2D> specular, float shininess);

public:
    virtual void Bind(std::string const& base_name) const override;
};

LEARN_OPENGL_NAMESPACE_END

#endif // !_LEARN_OPENGL_MATERIAL_TEXTURE_MATERIAL_HPP_
