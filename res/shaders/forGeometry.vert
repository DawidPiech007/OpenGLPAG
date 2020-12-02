#version 440

layout (location = 0) in vec3 aPos;

//out vec4 pos;

//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

void main()
{
    //pos = projection * view * model * vec4(aPos, 1.0);
    //vs_out.pos = vec4(aPos, 1.0);
    gl_Position = vec4(aPos, 1.0);
}