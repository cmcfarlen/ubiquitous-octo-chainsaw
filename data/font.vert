#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec4 fontColor;

uniform mat4 proj;
uniform vec4 color;

void main()
{
    gl_Position = proj * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    fontColor = color;
}
