#version 330 core

struct Light {
    vec3 position;
    vec3 direction;
    int pos_or_dir;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

out vec3 v_Position;
out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_LightPosOrDir;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform Light u_Light;

void main() {
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
    v_Position = vec3(u_View * u_Model * vec4(a_Position, 1.0));
    v_Normal = mat3(transpose(inverse(u_View * u_Model))) * a_Normal;
    v_TexCoord = a_TexCoord;
    // Transform world-space light position to view-space light position.
    switch (u_Light.pos_or_dir) {
        case 0:
            v_LightPosOrDir = vec3(u_View * vec4(u_Light.position, 1.0));
            break;
        case 1:
            v_LightPosOrDir = vec3(u_View * vec4(u_Light.direction, 1.0));
            break;
        default:
            v_LightPosOrDir = vec3(0.0);
            break;
    }
}
