#version 330 core
#ifndef PI
#define PI 3.141592653589793
#endif

layout (lines_adjacency) in;
layout (triangle_strip, max_vertices=32) out;


layout (std140) uniform camera
{
	mat4 uViewProjection;
	float uWorldToScreen;
};

in int vIndex[];
const int nbWidth = 5;
uniform float uWidth[nbWidth];
uniform float uAspectRatio;
uniform int uIndexCount;

/*float vecAngle(vec2 a, vec2 b)
{
	float dott = a.x * b.x + a.y * b.y;
	float det = a.x * b.y - a.y * b.y;

	if(dott > 0)
	return atan(det/dott);
	else 
	return PI;
}*/

float sineInOut(float t) 
{
	return -0.5 * (cos(PI * t) - 1.0);
}

vec2 offsetDir(vec2 dirIn, vec2 dirOut, float width)
{
	float w = clamp((1 - dot(dirIn, dirOut)) * 1.41, 1, 1.41);
	vec2 c = vec2(cross(vec3(-dirIn.xy, 1), vec3(dirOut.xy, 1)));
	c.x /= uAspectRatio;
	return normalize(c) * width * uWorldToScreen * 0.5 * w;
}

float width(int index, int count)
{
	float increment = 1.0 / float(nbWidth);

	float progression = float(index) / float(count);

	float prev = floor(progression / increment);
	float next = ceil(progression / increment);

	float curProg = progression / (increment * next);

	return mix(uWidth[int(prev)], uWidth[int(next)], sineInOut(curProg));
}

void main()
{

	vec2 dirIn = gl_in[1].gl_Position.xy - gl_in[0].gl_Position.xy;
	vec2 dir =  gl_in[2].gl_Position.xy - gl_in[1].gl_Position.xy;
	vec2 dirOut =  gl_in[3].gl_Position.xy - gl_in[2].gl_Position.xy;



	//begin of line
	if(length(dirIn) < 0.0001)
		dirIn =  dir;
	//end of line
	if(length(dirOut) < 0.0001)
		dirOut =  dir;
	
	//for cross product to work
	dirIn = normalize(dirIn);
	dirOut = normalize(dirOut);
	dir = normalize(dir);

	float widthBegin = width(vIndex[1], uIndexCount);
	float widthEnd = width(vIndex[2], uIndexCount);

	vec2 startOffset = offsetDir(dirIn, dir, widthBegin);
	vec2 endOffset = offsetDir(dir, dirOut, widthEnd);
	startOffset.x /= uAspectRatio;
	endOffset.x /= uAspectRatio;

	startOffset.y += float(vIndex[0]) * 0.01;

	gl_Position = gl_in[1].gl_Position + vec4(startOffset.xy, 0, 0);
	EmitVertex();
	gl_Position = gl_in[1].gl_Position - vec4(startOffset.xy, 0, 0);
	EmitVertex();
	gl_Position = gl_in[2].gl_Position + vec4(endOffset.xy, 0, 0);
	EmitVertex();
	gl_Position = gl_in[2].gl_Position - vec4(endOffset.xy, 0, 0);
	EmitVertex();




	EndPrimitive();
}