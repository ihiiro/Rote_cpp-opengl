#version 330 core
layout (location = 0) in vec3 apos;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec2 texture_coords;

out vec2 tex_coord;
out vec3 our_color;

void main() {
  gl_Position = vec4(apos, 1.0f);
  our_color = a_color;
  tex_coord = texture_coords;
}
