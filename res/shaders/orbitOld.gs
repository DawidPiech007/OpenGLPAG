#version 440

layout (points) in;
layout (line_strip, max_vertices = 20) out;

//uniform float r;
//uniform float vertexN;

//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

void main()
{
    for(float i = 0.0; i < vertexN; i = i+1)
    {
        float angle = i * angleSteps;
        float x = r * cos(angle);
        float y = r * sin(angle);
    
        gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(x, y, 0.0, 0.0));
        //gl_Position = projection * view * model * (vec4(0.0, 0.0, 0.0, 0.0) + vec4(x, y, 0.0, 0.0));
        //gl_Position = (gl_in[0].gl_Position + vec4(x, y, 0.0, 0.0));
        //gl_Position = gl_in[0].gl_Position + (projection * view * model * vec4(x, y, 0.0, 1.0)) - vec4(0.0, 0.0, 0.0, 0.0);
        EmitVertex();
    }

    gl_Position = gl_in[0].gl_Position + vec4(0.0, 0.3, 0.0, 0.0); 
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(0.3, 0.0, 0.0, 0.0); 
    EmitVertex();

    /*
    for(float i = 0.0; i < vertexN; i = i+1)
    {
        float angleSteps = 360.0 / vertexN;
        float angle = i * angleSteps;
        float x = r * cos(angleSteps);
        float y = r * sin(angleSteps);
    
        vec4 offset = projection * view * model * vec4(x, y, 0.0, 1.0);
        vec4 point = projection * view * model * gl_in[0].gl_Position;

        //gl_Position = offset + point;
        gl_Position = vec4(x, y, 0.0, 0.0);
        EmitVertex();
    }
    */

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