#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>

const struct {
  short x {800};
  short y {600};
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

  Shader shader("/home/hiro/rote/cpp-opengl/project/src/shader.vs", "/home/hiro/rote/cpp-opengl/project/src/shader.fs");

  // show maximum number of vertex attributes supported
  int nr_attributes ;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nr_attributes);
  std::cout << "Maximum nr of vertex attributes supported: " << nr_attributes << std::endl;

  float vertices[][18] {
    {
      .0f, .5f, .0f,  1.0f, .0f, .0f,
      -.5f, -.5f, .0f,   .0f, 1.0f, .0f,
      .5f, -.5f, .0f,   .0f, .0f, 1.0f,
    },
  };
  unsigned int vertex_array_object, vertex_buffer_object;

  // generate objects
  glGenVertexArrays(1, &  vertex_array_object);
  glGenBuffers(1, &vertex_buffer_object);

  // bind/configure objects
  glBindVertexArray(vertex_array_object);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // for color vertex attributes
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); does not fill the triangles */

  while (!glfwWindowShouldClose(window)) {
    // input
    process_input(window);

    // clearing
    glClearColor(.2f, .3f, .3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // rendering
    shader.use();
    shader.set_float("horizontal_offset_value", .5f);
    glBindVertexArray(vertex_array_object);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // check and call events and swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vertex_array_object);
  glDeleteBuffers(1, &vertex_buffer_object);

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
