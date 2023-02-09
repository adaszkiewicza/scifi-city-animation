#version 330 core
out vec4 FragColor;  

uniform vec3 constColor;
  
void main()
{
    FragColor = vec4(constColor.xyz, 1.0);
}