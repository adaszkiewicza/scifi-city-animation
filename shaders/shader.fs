#version 330 core
out vec4 FragColor;  

uniform vec4 constColor;
  
void main()
{
    FragColor = constColor;
}