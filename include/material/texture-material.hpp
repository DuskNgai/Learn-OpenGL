#ifndef _LEARN_OPENGL_MATERIAL_TEXTURE_MATERIAL_HPP_
#define _LEARN_OPENGL_MATERIAL_TEXTURE_MATERIAL_HPP_

#include <material/base-material.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

/// @brief The material that need textures.
class TextureMaterial : public Material {
public:
    std::shared_ptr<dusk::Texture2D> diffuse;
    std::shared_ptr<dusk::Texture2D> specular;
    float shininess{ 0.0f };

public:
    TextureMaterial(std::shared_ptr<dusk::Texture2D> diffuse, std::shared_ptr<dusk::Texture2D> specular, float shininess, std::shared_ptr<dusk::Shader> const& shader = nullptr);
    virtual ~TextureMaterial() = default;

public:
    template <typename... Args>
    static std::shared_ptr<TextureMaterial> create(Args&&... args) {
        return std::make_shared<TextureMaterial>(std::forward<Args>(args)...);
    }

    virtual void bind(std::string const& base_name) const override;
};

LEARN_OPENGL_NAMESPACE_END

#endif // !_LEARN_OPENGL_MATERIAL_TEXTURE_MATERIAL_HPP_
