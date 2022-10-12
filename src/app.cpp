#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

  float vertices[] {
    // positions      // colors         // texture coords
    .5f, .5f, .0f,    1.0f, .0f, .0f,   2.0f, 2.0f,
    .5f, -.5f, .0f,   .0f, 1.0f, .0f,   2.0f, .0f,
    -.5f, -.5f, .0f,  .0f, .0f, 1.0f,   .0f, .0f,
    -.5f, .5f, .0f,   1.0f, 1.0f, .0f,  .0f, 2.0f,
  };

  unsigned int indices[] {
    0, 1, 3, // first triangle
    1, 2, 3, // second triangle
  };

  unsigned int vertex_array_object;
  unsigned int vertex_buffer_object;
  unsigned int element_buffer_object;

  glGenVertexArrays(1, &vertex_array_object);
  glGenBuffers(1, &vertex_buffer_object);
  glGenBuffers(1, &element_buffer_object);

  glBindVertexArray(vertex_array_object);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  unsigned int container;
  unsigned int awesomeface;

  glGenTextures(1, &container);
  glGenTextures(1, &awesomeface);

  glBindTexture(GL_TEXTURE_2D, container);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width;
  int height;
  int number_of_color_channels;

  stbi_set_flip_vertically_on_load(true);

  // container
  unsigned char* data = stbi_load("../textures/container.jpg", &width, &height, &number_of_color_channels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
    std::cout << "Failed to load `container` texture" << std::endl;
  }
  stbi_image_free(data);

  // awesomeface
  glBindTexture(GL_TEXTURE_2D, awesomeface);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_REPEAT is the default behaviour for textures
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  data = stbi_load("../textures/awesomeface.png", &width, &height, &number_of_color_channels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
    std::cout << "Failed to load `awesomeface` texture" << std::endl;
  }
  stbi_image_free(data);

  // set uniforms
  shader.use();
  shader.set_int("container", 0);
  shader.set_int("awesomeface", 1);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, container);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, awesomeface);

  /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); does not fill the triangles */

  while (!glfwWindowShouldClose(window)) {
    // input
    process_input(window);

    // clearing
    glClearColor(.2f, .3f, .3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // rendering
    shader.use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // check and call events and swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vertex_array_object);
  glDeleteVertexArrays(1, &vertex_buffer_object);
  glDeleteBuffers(1, &element_buffer_object);

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
