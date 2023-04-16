
#version 330 core

out vec4 oColor;

in vec4 vColor;
in vec2 vTexCoords;
in float vTexIndex;

uniform sampler2D uTextures[16];

void main()
{
	vec4 color = texture(uTextures[int(round(vTexIndex))], vTexCoords) * vColor;

	if(color.a >= 0.01)
		color.rgb = vec3(1, 0, 0);

	oColor = color;
}