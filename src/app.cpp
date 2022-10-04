#include <iostream>
#include <cmath>
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
  Shader shader_green("/home/hiro/rote/cpp-opengl/project/src/shader.vs", "/home/hiro/rote/cpp-opengl/project/src/shader_green.fs");

  // show maximum number of vertex attributes supported
  int nr_attributes ;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nr_attributes);
  std::cout << "Maximum nr of vertex attributes supported: " << nr_attributes << std::endl;

  float vertices[][18] {
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
    shader.use();
    glBindVertexArray(vertex_array_objects[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    shader_green.use();
    float time_value = glfwGetTime();
    float green_value = (sin(time_value) / 2.0f) + .5f;
    float rgba_value[] {.0f, green_value, .0f, 1.0f};
    shader_green.set_float_sin("color_from_opengl_code", rgba_value);

    glBindVertexArray(vertex_array_objects[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // check and call events and swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(2, vertex_array_objects);
  glDeleteBuffers(2, vertex_buffer_objects);

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
