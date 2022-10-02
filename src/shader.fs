#version 330 core
in vec3 color_set_from_vertices_array;
out vec4 frag_color;
void main() {
  frag_color = vec4(color_set_from_vertices_array, 1.0f);
}
