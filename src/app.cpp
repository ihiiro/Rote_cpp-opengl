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

void framebuffer_size_callback(GLFWwindow* window, int width, int heigth) {
  glViewport(0, 0, width, heigth);
}

void process_input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

int main() {
  // initialize GLFW and configure
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create window and set as GLFW current context
  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // initialize GLAD
  if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress))) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // configure viewport
  glViewport(0, 0, 800, 600);

  // set GLFW callback function(this one resizes viewport
  // in response to window resizing event)
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // create vertex data
  float vertices[] = {
    -.5f, -.5f, .0f,
    .5f, -.5f, .0f,
    .0f, .5f, .0f,
  };
  // generate vertex buffer object, bind it, fill it with predefined vertex data
  unsigned int vertex_buffer_object;
  glGenBuffers(1, &vertex_buffer_object);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // create vertex shader, set source, compile shader
  unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &VERTEX_SHADER_SOURCE, NULL);
  glCompileShader(vertex_shader);
  // now for the fragment shader
  unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &FRAGMENT_SHADER_SOURCE, NULL);
  glCompileShader(fragment_shader);
  // check for shaders compilation success
  int success;
  char info_log[512];
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
    std::cout << "\nERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
  }
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
    std::cout << "\nERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
  }

  // create shader program, attach shaders to it, link shaders
  unsigned int shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  // check if linking shaders was successful
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program, 512, NULL, info_log);
    std::cout << "\nERROR::SHADER_PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
  }
  // shader_program is now used, delete the shader objects(no longer needed)
  glUseProgram(shader_program);
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  // tell OpenGL how to link vertex data to vertex shader's vertex attributes 
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // render loop
  while (!glfwWindowShouldClose(window)) {
    // input
    process_input(window);

    // rendering commands
    glClearColor(.2f, .3f, .3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // check and call events and swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
