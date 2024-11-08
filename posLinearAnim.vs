#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 targetPos;

uniform float alpha;

void main()
{
	gl_Position =  (1-alpha) * vec4(aPos, 1.0) + alpha * vec4(targetPos, 1.0) ;
}