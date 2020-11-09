#version 440

out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;


void main()
{
	vec4 colour = vec4(1.0f, 0.0f, 0.0f, 1.00f);  // nowe
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2); // stare
	FragColor = mix(texture(texture1, TexCoord), colour, 0.5);
}