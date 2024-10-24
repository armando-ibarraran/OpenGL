#version 330 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform float alfa;

void main()
{
    FragColor = vec4(alfa * lightColor * objectColor, 1.0);
}