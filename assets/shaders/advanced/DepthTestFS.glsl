#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform float u_Near;
uniform float u_Far;

// Convert harmonic depth to linear depth.
float linearized_depth(float depth) {
    // Back to NDC.
    float z = depth * 2.0 - 1.0;
    return (2.0 * u_Near * u_Far) / (u_Far + u_Near - z * (u_Far - u_Near));
}

void main() {
    float depth = linearized_depth(gl_FragCoord.z) / u_Far;
    color = vec4(vec3(depth), 1.0);
}
