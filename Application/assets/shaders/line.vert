#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in int aIndex;

uniform mat4 aTransform;
uniform vec4 aColor;


layout (std140) uniform camera
{
	mat4 uViewProjection;
	float uWorldToScreen;
};

out int vIndex;

void main()
{
	vIndex = aIndex;
	vec4 pos = aTransform * vec4(aPosition.xyz ,1);
	pos = vec4(pos.xyz * uWorldToScreen, 1);
	gl_Position = uViewProjection * pos;
}
