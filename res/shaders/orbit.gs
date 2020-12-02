#version 440

layout (points) in;
layout (line_strip, max_vertices = 8) out;

//in vec4 pos;

void main()
{
    gl_Position = gl_in[0].gl_Position + vec4(0.5, 0.0, 0.0, 0.0); 
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4( 0.4, 0.4, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position+ vec4( 0.0, 0.5, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position+ vec4( -0.4, 0.4, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position+ vec4(-0.5, 0.0, 0.0, 0.0); 
    EmitVertex();

    gl_Position = gl_in[0].gl_Position+ vec4( -0.4, -0.4, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position+ vec4( 0.0, -0.5, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position+ vec4( 0.4, -0.4, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}