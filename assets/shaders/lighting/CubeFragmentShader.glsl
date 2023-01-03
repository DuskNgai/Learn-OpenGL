#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_LightPos;

uniform vec3 u_ObjectColor;
uniform vec3 u_LightColor;

void main() {
    // Ambient lighting.
    float ambient_strength = 0.1;
    vec3 ambient = ambient_strength * u_LightColor;

    // Diffuse lighting.
    vec3 normal = normalize(v_Normal);
    vec3 light_dir = normalize(v_LightPos - v_Position); // From object to light.
    float cos_term = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = cos_term * u_LightColor;

    // Specular lighting.
    float specular_strength = 0.5;
    vec3 view_dir = normalize(-v_Position); // From object to camera.
    vec3 reflect_dir = reflect(-light_dir, normal);
    float specular_factor = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
    vec3 specular = specular_strength * specular_factor * u_LightColor;

    vec3 result = (ambient + diffuse + specular) * u_ObjectColor;
    color = vec4(result, 1.0);
}
