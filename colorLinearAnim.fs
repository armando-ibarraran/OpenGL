#version 330 core

out vec4 FragColor;

uniform vec4 startColor;
uniform vec4 endColor;
uniform float alpha;

void main()
{
	FragColor = (1-alpha)*startColor + alpha*endColor;
}