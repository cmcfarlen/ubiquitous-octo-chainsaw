#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec4 objectColor;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
   float ambientStrength = 0.1;
   float specularStrength = 0.5;
   vec4 ambient = ambientStrength * lightColor;

   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(lightPos - FragPos);

   float diff = max(dot(norm, lightDir), 0.0);
   vec4 diffuse = diff * lightColor;

   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm);

   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   vec4 specular = specularStrength * spec * lightColor;

   vec4 color = (ambient + diffuse + specular) * objectColor;
   FragColor = vec4(color.rgb, 1.0);
}
