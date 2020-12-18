#version 440

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture_diffuse1;
//uniform vec4 color;


void main()
{
	FragColor = texture(texture_diffuse1, TexCoord);
}