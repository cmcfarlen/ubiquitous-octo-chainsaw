#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec4 aColor;

out vec4 theColor;

uniform mat4 proj;
uniform mat4 view;

void main()
{
   gl_Position = proj * view * (vec4(aPos, 1.0));
   theColor = aColor;
}
