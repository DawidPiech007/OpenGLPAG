#version 440

out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
//uniform sampler2D texture2;
uniform vec4 color;


void main()
{
	//vec4 color = vec4(1.0f, 0.0f, 0.0f, 1.00f);  // nowe
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	// FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2); // mieszanie tekstur
	// FragColor = mix(texture(texture1, TexCoord), color, 0.5);                       // mieszanie tekstury z kolorem

	FragColor = texture(texture1, TexCoord);
}