#version 330 core
out vec4 frag_color;

in vec2 tex_coord;
in vec3 our_color;

uniform sampler2D container;
uniform sampler2D awesomeface;

void main() {
  // mix two textures
  frag_color = mix(texture(container, tex_coord), texture(awesomeface, vec2(-tex_coord.x, tex_coord.y)), .2f);
}
