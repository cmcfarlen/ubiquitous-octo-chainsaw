#version 330 core
layout (location=0) in vec3 pos;
layout (location=3) in vec3 aNormal;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec3 FragPos;
out vec3 Normal;

void main()
{
   gl_Position = proj * view * model * vec4(pos, 1);
   FragPos = vec3(model * vec4(pos, 1.0));
   Normal = aNormal;
}

