#version 440

out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform vec3 lightColor;


void main()
{
	FragColor = vec4(lightColor, 0.1);
}