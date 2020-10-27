#version 440

in vec3 pos;

out vec4 fragColor;

void main()  
{  
    fragColor = vec4(pos.x, pos.y, pos.z, 1.0f);  
}  