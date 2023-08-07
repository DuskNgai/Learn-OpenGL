#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

float sharp_kernel[9] = float[](
    -1, -1, -1,
    -1, 9, -1,
    -1, -1, -1
);

float blur_kernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16
);

float edge_kernel[9] = float[](
    1, 1, 1,
    1, -8, 1,
    1, 1, 1
);

const float offset = 1.0 / 100.0;
vec2 offsets[9] = vec2[](
    vec2(-offset, offset),  // Top-left
    vec2(0.0f, offset),     // Top
    vec2(offset, offset),   // Top-right
    vec2(-offset, 0.0f),    // Left
    vec2(0.0f, 0.0f),       // Center
    vec2(offset, 0.0f),     // Right
    vec2(-offset, -offset), // Down-left
    vec2(0.0f, -offset),    // Down
    vec2(offset, -offset)   // Down-right
);

void main() {
    vec4 tex_color = texture(u_Texture, v_TexCoord);

    // color = vec4(vec3(1.0 - tex_color), 1.0);

    // float gray_scale = 0.2126 * tex_color.r + 0.7152 * tex_color.g + 0.0722 * tex_color.b;
    // color = vec4(gray_scale, gray_scale, gray_scale, 1.0);

    vec3 neighbor_color[9];
    for (int i = 0; i < 9; i++) {
        neighbor_color[i] = vec3(texture(u_Texture, v_TexCoord.st + offsets[i]));
    }
    vec3 conv = vec3(0.0);
    for (int i = 0; i < 9; i++) {
        conv += neighbor_color[i] * edge_kernel[i];
    }

    color = vec4(conv, 1.0);
}
