#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Textures[1];

void main() {             
    color = texture(u_Textures[0], v_TexCoord);
}
