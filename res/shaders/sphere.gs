#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 400) out;

uniform float r;
uniform float vertexN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
  
void main() {    
    float PI = 3.14159f;

    float sectorCount = vertexN * 2;
    float stackCount  = vertexN * 3;

    float x, y, z, xy;

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            //vertices.push_back(x);
            //vertices.push_back(y);
            //vertices.push_back(z);
            gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(x, y, z, 0.0));
            EmitVertex();
        }
    }

    // generate CCW index list of sphere triangles
    //std::vector<int> indices;
    //int k1, k2;
    //for(int i = 0; i < stackCount; ++i)
    //{
    //    k1 = i * (sectorCount + 1);     // beginning of current stack
    //    k2 = k1 + sectorCount + 1;      // beginning of next stack
    //
    //    for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
    //    {
    //        // 2 triangles per sector excluding first and last stacks
    //        // k1 => k2 => k1+1
    //        if(i != 0)
    //        {
    //            indices.push_back(k1);
    //            indices.push_back(k2);
    //            indices.push_back(k1 + 1);
    //        }
    //
    //        // k1+1 => k2 => k2+1
    //        if(i != (stackCount-1))
    //        {
    //            indices.push_back(k1 + 1);
    //            indices.push_back(k2);
    //            indices.push_back(k2 + 1);
    //        }
    //    }
    //}
    
    EndPrimitive();
}    