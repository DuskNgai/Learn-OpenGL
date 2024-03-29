// Shader for batch rendering.
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_TilingScale;

uniform sampler2D u_Textures[32];

void main() {
    // Adding 0.5 to prevent floating point errors.
    color = texture(u_Textures[int(v_TexIndex + 0.5)], v_TexCoord * v_TilingScale) * v_Color;
}
