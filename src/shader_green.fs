#version 330 core
out vec4 frag_color;
uniform vec4 color_from_opengl_code;
void main() {
  frag_color = color_from_opengl_code;
}
