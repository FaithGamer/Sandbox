#version 330 core

layout (lines_adjacency) in;
layout (triangle_strip, max_vertices=4) out;


layout (std140) uniform camera
{
	mat4 uViewProjection;
	float uWorldToScreen;
};

in vec4 vColor[];
float lineWidth = 0.05;

float vecAngle(vec2 a, vec2 b)
{
	float dott = a.x * b.x + a.y * b.y;
	float det = a.x * b.y - a.y * b.y;

	if(dott > 0)
	return atan(det/dott);
	else 
	return 3.14159265359;
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


		/*float dott = dirIn.x * dir.x + dirIn.y * dir.y;
		float dett = dirIn.x * dir.y - dirIn.y * dir.y;

		float angle = atan(dett/dott);*/

		float w1 = clamp((1 - dot(dirIn, dir)) * 1.41, 1, 1.41);
		float w2 = clamp((1 - dot(dir, dirOut)) * 1.41, 1, 1.41);
		vec2 startOffset = normalize(vec2(cross(vec3(-dirIn.xy, 1), vec3(dir.xy, 1))).xy) * lineWidth * 0.5 * w1;
		vec2 endOffset = normalize(vec2(cross(vec3(-dir.xy, 1), vec3(dirOut.xy, 1))).xy) * lineWidth * 0.5 *w2;

		/*vec2 startOffset = -dirIn + dir;
		if(length(startOffset) < 0.0001)
		{
			startOffset = vec2(-dir.y, dir.x) * lineWidth * 0.5;
		}
		else
		{
			startOffset = normalize(startOffset) * lineWidth * 0.5;
		}
		vec2 endOffset = -dir + dirOut;
		if(length(endOffset) < 0.0001)
		{
			endOffset = vec2(-dir.y, dir.x) * lineWidth * 0.5;
		}
		else
		{
			endOffset = normalize(endOffset) * lineWidth * 0.5;
		}*/


		gl_Position = gl_in[1].gl_Position + vec4(startOffset.xy, 0, 0);
		EmitVertex();
		gl_Position = gl_in[1].gl_Position - vec4(startOffset.xy, 0, 0);
		EmitVertex();
		gl_Position = gl_in[2].gl_Position + vec4(endOffset.xy, 0, 0);
		EmitVertex();
		gl_Position = gl_in[2].gl_Position - vec4(endOffset.xy, 0, 0);
		EmitVertex();

		/*gl_Position = gl_in[0].gl_Position;// + vec4(startOffset.xy, 0, 0);
		EmitVertex();
		gl_Position = gl_in[1].gl_Position;// - vec4(startOffset.xy, 0, 0);
		EmitVertex();
		gl_Position = gl_in[2].gl_Position;// + vec4(endOffset.xy, 0, 0);
		EmitVertex();
		gl_Position = gl_in[3].gl_Position;// - vec4(endOffset.xy, 0, 0);
		EmitVertex();*/


	EndPrimitive();
}