#version 330 core

smooth in vec4 FragColorSmooth;
flat in vec4 FragColorFlat;

out vec4 FragColor;

uniform bool flatOn;

void main()
{
    if (flatOn)
        FragColor = FragColorFlat;
    else 
        FragColor = FragColorSmooth;
}