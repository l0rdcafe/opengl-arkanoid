#version 410 core
layout (location = 0) in vec3 aPos;

out vec3 color;
uniform vec3 aColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = model * view * projection * vec4(aPos, 1.0);
  color = aColor;
}
