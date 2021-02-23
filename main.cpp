#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaderinit.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const unsigned int screen_width = 1024;
const unsigned int screen_height = 768;

bool has_started = false;
bool is_moving = false;

const unsigned int n_rect_vertices = 6;
const unsigned int n_sphere_vertices = 360;
const unsigned int n_rows = 6;
const unsigned int n_cols = 5;

GLfloat sphere_radius = 1.0f;

float rect_vertices[] = {
  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
  -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
  -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f
};

unsigned int rect_indices[] = {
  0, 1, 3,
  1, 2, 3
};

glm::vec3 colors[n_cols] = {
  glm::vec3(1.0f, 0.0f, 0.5f),
  glm::vec3(0.5f, 1.0f, 0.0f),
  glm::vec3(0.0f, 0.5f, 1.0f),
  glm::vec3(0.5f, 0.0f, 0.5f),
  glm::vec3(1.0f, 0.5f, 0.0f)
};

glm::vec3 player_color = glm::vec3(1.0f, 1.0f, 0.0f);
glm::vec3 player_outline_color = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 sphere_color = glm::vec3(1.0f, 0.0f, 0.0f);

glm::vec3 player_default_position = glm::vec3(0.0f, -0.95f, 0.0f);
glm::vec3 sphere_default_position = player_default_position - glm::vec3(0.0f, 0.01f, 0.0f);

void handle_input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
	  glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && has_started) {
    is_moving = true;
  }
}

GLuint init_rect() {
  GLuint VAO, VBO, EBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rect_indices), rect_indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;
}

GLuint init_rect_outline() {
  GLuint VAO, VBO, EBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rect_indices), rect_indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return VAO;
}

GLuint init_sphere() {
  GLuint VAO, VBO, EBO;
  float vertices[] = {
    0.0f, 0.04f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.03f, -0.02f, 1.0f, 0.0f, 0.0f,
    0.02f, 0.03f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.02f, 0.03f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.03f, 0.02f, 1.0f, 0.0f, 0.0f,

    0.0f, 0.0f, -0.03f, 1.0f, 0.0f, 0.0f,
    0.02f, 0.0f, -0.02f, 1.0f, 0.0f, 0.0f,
    0.03f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.02f, 0.0f, 0.02f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, -0.03f, 1.0f, 0.0f, 0.0f,
    -0.02f, 0.0f, 0.02f, 1.0f, 0.0f, 0.0f,
    -0.03f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.02f, 0.0f, -0.02f, 1.0f, 0.0f, 0.0f,

    0.0f, -0.03f, -0.02f, 1.0f, 0.0f, 0.0f,
    0.02f, -0.03f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, -0.03f, 0.02f, 1.0f, 0.0f, 0.0f,
    -0.02f, -0.03f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, -0.04f, 0.0f, 1.0f, 0.0f, 0.0f,
  };

  unsigned int indices[] = {
    0, 1, 2,
    0, 2, 4,
    0, 4, 3,
    0, 1, 3,

    1, 5, 6,
    1, 6, 2,
    2, 6, 7,
    2, 7, 8,
    2, 8, 4,
    4, 8, 9,
    4, 9, 10,
    4, 10, 3,
    3, 10, 11,
    3, 11, 12,
    3, 12, 1,
    1, 12, 5,

    5, 13, 6,
    6, 13, 14,
    14, 6, 7,
    7, 14, 8,
    14, 8, 15,
    15, 8, 9,
    9, 15, 10,
    10, 16, 15,
    10, 16, 11,
    11, 16, 12,
    16, 12, 13,
    13, 12, 5,

    17, 13, 14,
    17, 14, 15,
    17, 15, 16,
    17, 16, 13,

    0, 1, 2,
    0, 2, 3,
    0, 3, 4,
    0, 4, 1,
    5, 1, 2,
    5, 2, 3,
    5, 3, 4,
    5, 4, 1
  };

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return VAO;
}

void draw_player(Shader shader) {
  GLuint rect_vao = init_rect();
  glBindVertexArray(rect_vao);
  glm::mat4 player_model = glm::mat4(1.0f);
  player_model = glm::translate(player_model, player_default_position);
  player_model = glm::scale(player_model, glm::vec3(0.5f, 0.15f, 0.2f));
  glUniform3fv(glGetUniformLocation(shader.id, "aColor"), 1, glm::value_ptr(player_color));
  glUniformMatrix4fv(glGetUniformLocation(shader.id, "model"), 1, GL_FALSE, &player_model[0][0]);
  glDrawElements(GL_TRIANGLES, n_rect_vertices, GL_UNSIGNED_INT, 0);

  GLuint rect_outline_vao_y = init_rect_outline();
  glBindVertexArray(rect_outline_vao_y);
  glm::mat4 player_outline_model_y = glm::mat4(1.0f);
  player_outline_model_y = glm::translate(player_outline_model_y, player_default_position);
  player_outline_model_y = glm::scale(player_outline_model_y, glm::vec3(0.505f, 0.15f, 0.2f));
  glUniform3fv(glGetUniformLocation(shader.id, "aColor"), 1, glm::value_ptr(player_outline_color));
  glUniformMatrix4fv(glGetUniformLocation(shader.id, "model"), 1, GL_FALSE, &player_outline_model_y[0][0]);
  glLineWidth(40);
  glDrawArrays(GL_LINES, 0, 8);

  GLuint rect_outline_vao_x = init_rect_outline();
  glBindVertexArray(rect_outline_vao_x);
  glm::mat4 player_outline_model_x = glm::mat4(1.0f);
  player_outline_model_x = glm::translate(player_outline_model_x, player_default_position + glm::vec3(-0.25f, 0.075f, 0.0f));
  player_outline_model_x = glm::scale(player_outline_model_x, glm::vec3(0.505f, 0.15f, 0.2f));
  player_outline_model_x = glm::rotate(player_outline_model_x, glm::radians(180.0f), glm::vec3(1.0f, 1.0f, 0.0f));
  glUniform3fv(glGetUniformLocation(shader.id, "aColor"), 1, glm::value_ptr(player_outline_color));
  glUniformMatrix4fv(glGetUniformLocation(shader.id, "model"), 1, GL_FALSE, &player_outline_model_x[0][0]);
  glLineWidth(40);
  glDrawArrays(GL_LINES, 0, 8);

  GLuint sphere_vao = init_sphere();
  glBindVertexArray(sphere_vao);
  glm::mat4 sphere_model = glm::mat4(1.0f);
  sphere_model = glm::translate(sphere_model, sphere_default_position);

  if (is_moving) {
    sphere_model = glm::translate(sphere_model, sphere_default_position + glm::vec3(0.0f, -2.0f + (float)glfwGetTime() * 0.8f, 0.0f));
  }

  sphere_model = glm::scale(sphere_model, glm::vec3(0.5f, 0.5f, 1.0f));
  glUniform3fv(glGetUniformLocation(shader.id, "aColor"), 1, glm::value_ptr(sphere_color));
  glUniformMatrix4fv(glGetUniformLocation(shader.id, "model"), 1, GL_FALSE, &sphere_model[0][0]);
  glDrawElements(GL_TRIANGLES, n_sphere_vertices, GL_UNSIGNED_INT, 0);
}

void render(Shader shader, GLFWwindow* window) {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  shader.use();

  // draw grid of bricks
  for (int x = 0; x < n_rows; x++) {
    for (int y = 0; y < n_cols; y++) {
      GLuint VAO = init_rect();
      glBindVertexArray(VAO);
      glm::mat4 rect_model = glm::mat4(1.0f);
      rect_model = glm::translate(rect_model, glm::vec3((-0.25f * x) + 0.625f, (-0.25f * y) + 0.75f, 0.0f));
      rect_model = glm::scale(rect_model, glm::vec3(0.2f, 0.2f, 0.2f));

      glUniform3fv(glGetUniformLocation(shader.id, "aColor"), 1, glm::value_ptr(colors[y]));

      glUniformMatrix4fv(glGetUniformLocation(shader.id, "model"), 1, GL_FALSE, &rect_model[0][0]);
      glDrawElements(GL_TRIANGLES, n_rect_vertices, GL_UNSIGNED_INT, 0);
    }
  }

  draw_player(shader);

  glfwSwapBuffers(window);
  glfwPollEvents();
}

void init(Shader shader) {
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.5f, 0.0f));
  projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);

  shader.use();
  glUniformMatrix4fv(glGetUniformLocation(shader.id, "model"), 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(glGetUniformLocation(shader.id, "view"), 1, GL_FALSE, &view[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(shader.id, "projection"), 1, GL_FALSE, &projection[0][0]);
}

int main() {
	if (!glfwInit()) {
		fprintf(stderr, "Failed to init GLFW");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "OpenGLMidterm", NULL, NULL);

	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to init GLAD" << endl;
		return -1;
	}

	Shader ourShader("shader.vs", "shader.fs");
	glEnable(GL_DEPTH_TEST);
	init(ourShader);

	while (!glfwWindowShouldClose(window)) {
	  float current_frame = glfwGetTime();
	  handle_input(window);

	  if (current_frame >= 2.5f) {
	    has_started = true;
	  }

	  render(ourShader, window);
  }

  glfwTerminate();
  return 0;
}
