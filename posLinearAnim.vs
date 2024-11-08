#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 targetPos;

uniform float alpha;

void main()
{
	gl_Position = alpha * vec4(aPos, 0.0, 1.0) + (1-alpha) * vec4(targetPos, 0.0, 1.0) ;
}