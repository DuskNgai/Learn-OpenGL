#ifndef _LEARN_OPENGL_MATERIAL_PURE_COLOR_MATERIAL_HPP_
#define _LEARN_OPENGL_MATERIAL_PURE_COLOR_MATERIAL_HPP_

#include <Material/BaseMaterial.hpp>

LEARN_OPENGL_NAMESPACE_BEGIN

/// @brief The material only has pure color.
class PureColorMaterial : public Material {
public:
    glm::vec3 ambient{0.0f, 0.0f, 0.0f};
    glm::vec3 diffuse{0.0f, 0.0f, 0.0f};
    glm::vec3 specular{0.0f, 0.0f, 0.0f};
    float shininess{0.0f};

public:
    PureColorMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
    PureColorMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, std::shared_ptr<Dusk::Shader> shader);

public:
    static std::shared_ptr<PureColorMaterial> Create(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
    static std::shared_ptr<PureColorMaterial> Create(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, std::shared_ptr<Dusk::Shader> shader);

    virtual void Bind(std::string const& base_name) const override;
};

static std::vector<std::pair<std::string, std::shared_ptr<Material>>> s_named_pure_color_materials {
    {"Corol",          PureColorMaterial::Create({1.0f, 0.5f, 0.31f},               {1.0f, 0.5f, 0.31f},               {0.5f, 0.5f, 0.5f},                      32.0f)},
    {"Emerald",        PureColorMaterial::Create({0.0215f, 0.1745f, 0.0215f},       {0.07568f, 0.61424f, 0.07568f},    {0.633f, 0.727811f, 0.633f},             76.8f)},
    {"Jade",           PureColorMaterial::Create({0.135f, 0.2225f, 0.1575f},        {0.54f, 0.89f, 0.63f},             {0.316228f, 0.316228f, 0.316228f},       12.8f)},
    {"Obsidian",       PureColorMaterial::Create({0.05375f, 0.05f, 0.06625f},       {0.18275f, 0.17f, 0.22525f},       {0.332741f, 0.328634f, 0.346435f},       38.4f)},
    {"Pearl",          PureColorMaterial::Create({0.25f, 0.20725f, 0.20725f},       {1.0f, 0.829f, 0.829f},            {0.296648f, 0.296648f, 0.296648f},       12.7f)},
    {"Ruby",           PureColorMaterial::Create({0.1745f, 0.01175f, 0.01175f},     {0.61424f, 0.04136f, 0.04136f},    {0.727811f, 0.626959f, 0.626959f},       76.8f)},
    {"Turquoise",      PureColorMaterial::Create({0.1f, 0.18725f, 0.1745f},         {0.396f, 0.74151f, 0.69102f},      {0.297254f, 0.30829f, 0.306678f},        12.8f)},
    {"Brass",          PureColorMaterial::Create({0.329412f, 0.223529f, 0.027451f}, {0.780392f, 0.568627f, 0.113725f}, {0.992157f, 0.941176f, 0.807843f},       27.9f)},
    {"Bronze",         PureColorMaterial::Create({0.2125f, 0.1275f, 0.054f},        {0.714f, 0.4284f, 0.18144f},       {0.393548f, 0.271906f, 0.166721f},       25.6f)},
    {"Chrome",         PureColorMaterial::Create({0.25f, 0.25f, 0.25f},             {0.4f, 0.4f, 0.4f},                {0.774597f, 0.774597f, 0.774597f},       76.8f)},
    {"Copper",         PureColorMaterial::Create({0.19125f, 0.0735f, 0.0225f},      {0.7038f, 0.27048f, 0.0828f},      {0.256777f, 0.137622f, 0.086014f},       12.8f)},
    {"Gold",           PureColorMaterial::Create({0.24725f, 0.1995f, 0.0745f},      {0.75164f, 0.60648f, 0.22648f},    {0.628281f, 0.555802f, 0.366065f},       51.2f)},
    {"Silver",         PureColorMaterial::Create({0.19225f, 0.19225f, 0.19225f},    {0.50754f, 0.50754f, 0.50754f},    {0.508273f, 0.508273f, 0.508273f},       51.2f)},
    {"Black plastic",  PureColorMaterial::Create({0.0f, 0.0f, 0.0f},                {0.01f, 0.01f, 0.01f},             {0.50f, 0.50f, 0.50f},                   32.0f)},
    {"White plastic",  PureColorMaterial::Create({0.0f, 0.0f, 0.0f},                {0.55f, 0.55f, 0.55f},             {0.70f, 0.70f, 0.70f},                   32.0f)},
    {"Red plastic",    PureColorMaterial::Create({0.0f, 0.0f, 0.0f},                {0.5f, 0.0f, 0.0f},                {0.7f, 0.6f, 0.6f},                      32.0f)},
    {"Yellow plastic", PureColorMaterial::Create({0.0f, 0.0f, 0.0f},                {0.5f, 0.5f, 0.0f},                {0.60f, 0.60f, 0.50f},                   32.0f)},
    {"Green plastic",  PureColorMaterial::Create({0.0f, 0.0f, 0.0f},                {0.1f, 0.35f, 0.1f},               {0.45f, 0.55f, 0.45f},                   32.0f)},
    {"Cyan plastic",   PureColorMaterial::Create({0.0f, 0.1f, 0.06f},               {0.0f, 0.50980392f, 0.50980392f},  {0.50196078f, 0.50196078f, 0.50196078f}, 32.0f)},
    {"Black rubber",   PureColorMaterial::Create({0.02f, 0.02f, 0.02f},             {0.01f, 0.01f, 0.01f},             {0.4f, 0.4f, 0.4f},                      10.0f)},
    {"White rubber",   PureColorMaterial::Create({0.05f, 0.05f, 0.05f},             {0.5f, 0.5f, 0.5f},                {0.7f, 0.7f, 0.7f},                      10.0f)},
    {"Red rubber",     PureColorMaterial::Create({0.05f, 0.0f, 0.0f},               {0.5f, 0.4f, 0.4f},                {0.7f, 0.04f, 0.04f},                    10.0f)},
    {"Yellow rubber",  PureColorMaterial::Create({0.05f, 0.05f, 0.0f},              {0.5f, 0.5f, 0.4f},                {0.7f, 0.7f, 0.04f},                     10.0f)},
    {"Green rubber",   PureColorMaterial::Create({0.0f, 0.05f, 0.0f},               {0.4f, 0.5f, 0.4f},                {0.04f, 0.7f, 0.04f},                    10.0f)},
    {"Cyan rubber",    PureColorMaterial::Create({0.0f, 0.05f, 0.05f},              {0.4f, 0.5f, 0.5f},                {0.04f, 0.7f, 0.7f},                     10.0f)}
};

LEARN_OPENGL_NAMESPACE_END

#endif // !_LEARN_OPENGL_MATERIAL_PURE_COLOR_MATERIAL_HPP_
