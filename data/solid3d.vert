#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 2) in vec4 col;

out vec4 theColor;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main()
{
   gl_Position = proj * view * model * vec4(pos, 1);
   theColor = col;

}
