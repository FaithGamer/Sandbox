#version 330 core

layout (location = 0) in vec3 aPosition;

uniform mat4 aTransform;
uniform vec4 aColor;


layout (std140) uniform camera
{
	mat4 uViewProjection;
	float uWorldToScreen;
};

out vec4 vColor;


void main()
{
	vColor = vec4(1, 1, 1, 1);
	vec4 pos = aTransform * vec4(aPosition.xyz ,1);
	pos = vec4(pos.xyz * uWorldToScreen, 1);
	gl_Position = uViewProjection * pos;
}
