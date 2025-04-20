#version 330 core

layout (lines) in;
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

    col2 = vec3(0.0f, 0.7f, 0.7f);
    vec4 add = normalize(vec4(gl_in[0].gl_Position.y-gl_in[1].gl_Position.y,gl_in[1].gl_Position.x-gl_in[0].gl_Position.x,0,0)) * data_in[0].scale;
    
    
    gl_Position = gl_in[0].gl_Position + add;
    EmitVertex();
    gl_Position = gl_in[0].gl_Position - add;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position - add;
    EmitVertex();

    EndPrimitive();

   gl_Position = gl_in[1].gl_Position + add;
   EmitVertex();
   gl_Position = gl_in[1].gl_Position - add;
   EmitVertex();
   gl_Position = gl_in[0].gl_Position + add;
   EmitVertex();
  
   EndPrimitive();
}