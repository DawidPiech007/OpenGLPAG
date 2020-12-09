#version 440 core

layout (points) in;
layout (triangle_strip, max_vertices = 256) out;

uniform float r;
uniform float vertexN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
  
void main() {    
    float PI = 3.14159f;

    const int sectorCount = 7;  //int(vertexN) * 3;
    const int stackCount  = 7;  //int(vertexN) * 2;
    //vec3 vertices[stackCount][sectorCount]; 
    vec3 vertices[(stackCount+1) * (sectorCount+1)];

    float x, y, z, xy;

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = r * cos(stackAngle);             // r * cos(u)
        z = r * sin(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cos(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sin(sectorAngle);             // r * cos(u) * sin(v)
            
            //vertices[i][j] = vec3(x, y, z);
            vertices[i * (sectorCount + 1) + j] = vec3(x, y, z);

            //vertices.push_back(x);
            //vertices.push_back(y);
            //vertices.push_back(z);
            //gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(x, y, z, 0.0));
            //EmitVertex();
        }
    }

    // generate CCW index list of sphere triangles
    //vector<int> indices;
    int k1, k2;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack
    
        //for(int j = 0; j < sectorCount; ++j)
        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if(i != 0)
            {
                gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(vertices[k1+1].x, vertices[k1+1].y, vertices[k1+1].z, 0.0));
                EmitVertex();
                gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(vertices[k2].x, vertices[k2].y, vertices[k2].z, 0.0));
                EmitVertex();
                gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(vertices[k1].x, vertices[k1].y, vertices[k1].z, 0.0));
                EmitVertex();
            }
            //{
            //    indices.push_back(k1);
            //    indices.push_back(k2);
            //    indices.push_back(k1 + 1);
            //}
            //
            // k1+1 => k2 => k2+1
            if(i != (stackCount-1))
            {
                gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(vertices[k2+1].x, vertices[k2+1].y, vertices[k2+1].z, 0.0));
                EmitVertex();
                gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(vertices[k2].x, vertices[k2].y, vertices[k2].z, 0.0));
                EmitVertex();
                gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(vertices[k1+1].x, vertices[k1+1].y, vertices[k1+1].z, 0.0));
                EmitVertex();
            }
            //{
            //    indices.push_back(k1 + 1);
            //    indices.push_back(k2);
            //    indices.push_back(k2 + 1);
            //}

            //gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(vertices[i][j].x, vertices[i][j].y, vertices[i][j].z, 0.0));
            //gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(vertices[i * sectorCount + j].x, vertices[i * sectorCount + j].y, vertices[i * sectorCount + j].z, 0.0));
            //EmitVertex();
        }
    }
    
    EndPrimitive();
}    