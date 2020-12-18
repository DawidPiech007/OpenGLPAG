#version 440

layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 aInstanceMatrix;

out vec2 TexCoord;

//uniform mat4 transform;
//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    //gl_Position = transform * vec4(aPos, 1.0);
    //gl_Position = projection * view * model * vec4(aPos, 1.0);
	//TexCoord = vec2(aTexCoord.x, aTexCoord.y);

    gl_Position = projection * view * aInstanceMatrix * vec4(aPos, 1.0);
    TexCoord = aTexCoords;   
}