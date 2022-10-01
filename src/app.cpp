#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// the code is in GLSL (OpenGL Shader Language)
const char* VERTEX_SHADER_SOURCE = "#version 330 core\n"
                                   "layout (location = 0) in vec3 apos;\n"
                                   "void main() {\n"
                                   "  gl_Position = vec4(apos.x, apos.y, apos.z, 1.0);\n"
                                   "}\0";
const char* FRAGMENT_SHADER_SOURCE = "#version 330 core\n"
                                     "out vec4 frag_color;\n"
                                     "void main() {\n"
                                     "  frag_color = vec4(1.0f, .5f, .2f, 1.0f);\n"
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

  unsigned int shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program, INFO_LOG_SIZE, NULL, info_log);
    std::cout << "\nERROR::SHADER_PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
  }
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  float vertices[] = {
    -.5f, -.5f, .0f,
    .5f, -.5f, .0f,
    .0f, .5f, .0f,
  };
  unsigned int vertex_buffer_object;
  unsigned int vertex_array_object;

  glGenBuffers(1, &vertex_buffer_object);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenVertexArrays(1, &vertex_array_object);
  glBindVertexArray(vertex_array_object);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  while (!glfwWindowShouldClose(window)) {
    // input
    process_input(window);

    // rendering commands
    glClearColor(.2f, .3f, .3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader_program);
    glBindVertexArray(vertex_array_object);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // check and call events and swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

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
