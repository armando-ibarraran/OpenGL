#version 330 core

out vec4 FragColor;

uniform vec4 startColor;
uniform vec4 endColor;
uniform float alpha;

void main()
{
	FragColor = alpha*startColor + (1-alpha)*endColor;
}