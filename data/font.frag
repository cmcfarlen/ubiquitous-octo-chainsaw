#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;
in vec4 fontColor;

uniform sampler2D ourTexture;

void main()
{
    FragColor = vec4(fontColor.rgb, texture(ourTexture, TexCoord).r);
    //FragColor = texture(ourTexture, TexCoord).rrrr;
}
