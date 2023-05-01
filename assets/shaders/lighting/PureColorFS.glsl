#version 330 core

struct PureColorMaterial {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;
    int pos_or_dir;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_LightPosOrDir;

uniform PureColorMaterial u_Material;
uniform Light u_Light;

void main() {
    // Ambient lighting.
    vec3 ambient = u_Light.ambient * u_Material.ambient;

    // Diffuse lighting.
    vec3 normal = normalize(v_Normal);
    vec3 light_dir = vec3(0.0);
    switch (u_Light.pos_or_dir) {
        case 0:
            light_dir = normalize(v_LightPosOrDir - v_Position);
            break;
        case 1:
             light_dir = normalize(-v_LightPosOrDir);
            break;
        default:
            break;
    }
    float cos_term = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = u_Light.diffuse * cos_term * u_Material.diffuse;

    // Specular lighting.
    vec3 view_dir = normalize(-v_Position); // From object to camera.
    vec3 reflect_dir = reflect(-light_dir, normal);
    float specular_factor = pow(max(dot(view_dir, reflect_dir), 0.0), u_Material.shininess);
    vec3 specular = u_Light.specular * specular_factor * u_Material.specular;

    // Attenuation.
    float attenuation = 1.0;
    if (u_Light.pos_or_dir == 0) {
        float dist = length(v_LightPosOrDir - v_Position);
        attenuation = 1.0 / (1.0 + 0.09 * dist + 0.032 * dist * dist);
    }

    vec3 result = (ambient + diffuse + specular) * attenuation;
    color = vec4(result, 1.0);
}
