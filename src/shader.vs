#version 330 core
layout (location = 0) in vec3 apos;
layout (location = 1) in vec3 a_color;
out vec3 color_set_from_vertices_array;

uniform float horizontal_offset_value;

void main() {
  gl_Position = vec4(apos.x + horizontal_offset_value, apos.yz, 1.0);
  color_set_from_vertices_array = a_color;
}
