#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

out vec3 col2;

in DATA
{
    vec3 col;
    float scale;
    float scale2;
    mat2 r;
} data_in[];

void main()
{
    col2 = data_in[0].col;

    const int vertexCount =12;
    vec2 vertices[4] = vec2[4](
      vec2(  -data_in[0].scale2,  -data_in[0].scale2),
      vec2(  -data_in[0].scale2,  data_in[0].scale2),
      vec2(  data_in[0].scale2,  data_in[0].scale2),
      vec2(  data_in[0].scale2,  -data_in[0].scale2)
    );
    
    gl_Position = gl_in[0].gl_Position + vec4((data_in[0].r) * vertices[0],0,0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4((data_in[0].r) * vertices[1],0,0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4((data_in[0].r) * vertices[2],0,0);
    EmitVertex();

    EndPrimitive();
    gl_Position = gl_in[0].gl_Position + vec4((data_in[0].r) * vertices[0],0,0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4((data_in[0].r) * vertices[3],0,0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4((data_in[0].r) * vertices[2],0,0);
    EmitVertex();

    EndPrimitive();


    
}