#version 440

out vec4 FragColor;

in vec2 TexCoords;

// texture samplers
//uniform sampler2D texture1;
//uniform sampler2D texture2;

uniform sampler2D texture_diffuse1;

void main()
{
	FragColor = vec4(texture(texture_diffuse1, TexCoords).r, texture(texture_diffuse1, TexCoords).g, texture(texture_diffuse1, TexCoords).b, texture(texture_diffuse1, TexCoords).r);
}