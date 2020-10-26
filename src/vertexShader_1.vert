#version 440

layout (location = 0) in vec3 vertexPosition;

out vec3 pos;

void main()  
{  
    pos = vertexPosition;

    gl_Position = vec4(vertexPosition.x, vertexPosition.y, vertexPosition.z, 1.0f);  
} 