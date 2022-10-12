#version 330 core
out vec4 frag_color;

in vec2 tex_coord;
in vec3 our_color;

uniform sampler2D our_texture;

void main() {
  frag_color = texture(our_texture, tex_coord) * vec4(our_color, 1.0f);
}
