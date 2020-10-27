#version 440

layout (location = 0) in vec3 vertexPosition;

out vec3 pos;

uniform mat4 wvp;

void main()  
{  
    pos = vertexPosition;

    gl_Position = wvp * vec4(vertexPosition, 1.0f);  
}  