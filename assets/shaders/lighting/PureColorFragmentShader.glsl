#version 330 core

struct PureColorMaterial {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_LightPos;

uniform PureColorMaterial u_Material;
uniform Light u_Light;

void main() {
    // Ambient lighting.
    vec3 ambient = u_Light.ambient * u_Material.ambient;

    // Diffuse lighting.
    vec3 normal = normalize(v_Normal);
    vec3 light_dir = normalize(v_LightPos - v_Position); // From object to light.
    float cos_term = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = u_Light.diffuse * cos_term * u_Material.diffuse;

    // Specular lighting.
    vec3 view_dir = normalize(-v_Position); // From object to camera.
    vec3 reflect_dir = reflect(-light_dir, normal);
    float specular_factor = pow(max(dot(view_dir, reflect_dir), 0.0), u_Material.shininess);
    vec3 specular = u_Light.specular * specular_factor * u_Material.specular;

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);
}
