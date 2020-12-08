#version 440

layout (points) in;
layout (line_strip, max_vertices = 20) out;


uniform float r;
uniform float vertexN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    float angleSteps = 6.28 / 4;

    float angle = 0 * angleSteps;
    float x = r * cos(angle);
    float y = r * sin(angle);
    //gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(0.0, r, 0.0, 0.0));
    //gl_Position = (vec4(0.0, r, -1.0, 1.0));
    //EmitVertex();

    angle = 1 * angleSteps;
    x = r * cos(angle);
    y = r * sin(angle);
    //gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(r, 0.0, 0.0, 0.0));
    //gl_Position = (vec4(r, 0.0, -1.0, 1.0));
    //EmitVertex();

    angle = 2 * angleSteps;
    x = r * cos(angle);
    y = r * sin(angle);
    //gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(0.0, -r, 0.0, 0.0));
    gl_Position = projection * view * model * (vec4(0.0, -r, 0.0, 1.0));
    EmitVertex();

    angle = 3 * angleSteps;
    x = r * cos(angle);
    y = r * sin(angle);
    //gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(-r, 0.0, 0.0, 0.0));
    gl_Position = projection * view * model * (vec4(-r, 0.0, 0.0, 1.0));
    EmitVertex();


    angle = 0 * angleSteps;
    x = r * cos(angle);
    y = r * sin(angle);
    //gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(0.0, r, 0.0, 0.0));
    gl_Position = projection * view * model * (vec4(0.0, r, 0.0, 1.0));
    EmitVertex();

    EndPrimitive();
}