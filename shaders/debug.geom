#version 330 core

layout (lines) in;
layout (line_strip, max_vertices = 12) out;

uniform vec3 p1;
uniform vec3 p2;
uniform mat4 mtr;
      uniform vec3 col;
      
      out vec3 col2;
void main()
{
col2 = col;
    
    gl_Position =mtr* vec4(p1,1);
    EmitVertex();
    gl_Position =mtr*vec4(p2,1);
    EmitVertex();

    EndPrimitive();


}