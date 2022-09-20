#version 330 core

out vec4 color;
in vec3 vPosition;
in vec4 vColor;


void main()
{
    color = vColor;
}