#version 440

layout (points) in;
layout (line_strip, max_vertices = 100) out;


uniform float r;
uniform float vertexN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    float vertexNNew = vertexN*4;
    float angleSteps = 6.28 / vertexNNew;

    for(float i = 0.0; i < vertexNNew; i = i+1)
    {
        float angle = i * angleSteps;
        float x = r * cos(angle);
        float z = r * sin(angle);
    
        gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(x, 0.0, z, 0.0));
        EmitVertex();
    }

    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(r, 0.0, 0.0, 0.0));
    EmitVertex();

    EndPrimitive();
}