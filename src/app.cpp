#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// the code is in GLSL (OpenGL Shader Language)
const char* VERTEX_SHADER_SOURCE = "#version 330 core\n"
                                   "layout (location = 0) in vec3 apos;\n"
                                   "layout (location = 1) in vec3 a_color;\n"
                                   "out vec3 color_set_from_vertices_array;\n"
                                   "void main() {\n"
                                   "  gl_Position = vec4(apos, 1.0);\n"
                                   "  color_set_from_vertices_array = a_color;\n"
                                   "}\0";
const char* FRAGMENT_SHADER_SOURCE = "#version 330 core\n"
                                     "in vec3 color_set_from_vertices_array;\n"
                                     "out vec4 frag_color;\n"
                                     "void main() {\n"
                                     "  frag_color = vec4(color_set_from_vertices_array, 1.0f);\n"
                                     "}\0";
const char* FRAGMENT_SHADER_SOURCE_GREEN = "#version 330 core\n"
                                            "out vec4 frag_color;\n"
                                            "uniform vec4 color_from_opengl_code;\n"
                                            "void main() {\n"
                                            "  frag_color = color_from_opengl_code;\n"
                                            "}\0";

// non-source constants
const short INFO_LOG_SIZE = 512;
const struct {
  short x = 800;
  short y = 600;
} SIZE;

// function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int heigth);
void process_input(GLFWwindow* window);

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(SIZE.x, SIZE.y, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress))) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &VERTEX_SHADER_SOURCE, NULL);
  glCompileShader(vertex_shader);
  int success;
  char info_log[INFO_LOG_SIZE];
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex_shader, INFO_LOG_SIZE, NULL, info_log);
    std::cout << "\nERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
  }

  unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &FRAGMENT_SHADER_SOURCE, NULL);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment_shader, INFO_LOG_SIZE, NULL, info_log);
    std::cout << "\nERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
  }

  unsigned int fragment_shader_yellow = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader_yellow, 1, &FRAGMENT_SHADER_SOURCE_GREEN, NULL);
  glCompileShader(fragment_shader_yellow);
  glGetShaderiv(fragment_shader_yellow, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment_shader_yellow, INFO_LOG_SIZE, NULL, info_log);
    std::cout << "\nERROR::SHADER::FRAGMENT_YELLOW::COMPILATION_FAILED\n" << info_log << std::endl;
  }

  unsigned int shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program, INFO_LOG_SIZE, NULL, info_log);
    std::cout << "\nERROR::SHADER_PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
  }

  unsigned int shader_program_green = glCreateProgram();
  glAttachShader(shader_program_green, vertex_shader);
  glAttachShader(shader_program_green, fragment_shader_yellow);
  glLinkProgram(shader_program_green);
  glGetProgramiv(shader_program_green, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program_green, INFO_LOG_SIZE, NULL, info_log);
    std::cout << "\nERROR::SHADER_PROGRAM_YELLOW::LINKING_FAILED\n" << info_log << std::endl;
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  glDeleteShader(fragment_shader_yellow);

  // show maximum number of vertex attributes supported
  int nr_attributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nr_attributes);
  std::cout << "Maximum nr of vertex attributes supported: " << nr_attributes << std::endl;



  float vertices[][18] = {
    {
      1.0f, -.5f, .0f,  1.0f, .0f, .0f,
      .1f, -.5f, .0f,   .0f, 1.0f, .0f,
      .55f, .5f, .0f,   .0f, .0f, 1.0f,
    },
    {
      -1.0f, -.5f, .0f,
      -.1f, -.5f, .0f,
      -.55f, .5f, .0f,
    },
  };
  unsigned int vertex_array_objects[2], vertex_buffer_objects[2];

  // generate objects
  glGenVertexArrays(2, vertex_array_objects);
  glGenBuffers(2, vertex_buffer_objects);

  // bind/configure objects
  glBindVertexArray(vertex_array_objects[0]);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]), vertices[0], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // for color vertex attributes
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(vertex_array_objects[1]);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[1]), vertices[1], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); does not fill the triangles */

  while (!glfwWindowShouldClose(window)) {
    // input
    process_input(window);

    // clearing
    glClearColor(.2f, .3f, .3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // rendering
    glUseProgram(shader_program);
    glBindVertexArray(vertex_array_objects[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(shader_program_green);
    float time_value = glfwGetTime();
    float green_value = (sin(time_value) / 2.0f) + .5f;
    int vertex_color_location = glGetUniformLocation(shader_program_green, "color_from_opengl_code");
    glUniform4f(vertex_color_location, .0f, green_value, .0f, 1.0f);

    glBindVertexArray(vertex_array_objects[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // check and call events and swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(2, vertex_array_objects);
  glDeleteBuffers(2, vertex_buffer_objects);
  glDeleteProgram(shader_program);
  glDeleteProgram(shader_program_green);

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int heigth) {
  glViewport(0, 0, width, heigth);
}

void process_input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
