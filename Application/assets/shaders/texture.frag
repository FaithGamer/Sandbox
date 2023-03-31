#version 330 core

out vec4 color;
in vec2 vTexCoords;

uniform sampler2D tex;

void main()
{
    color = texture(tex, vTexCoords);
    if(color.a < 0.1)
        discard;
}