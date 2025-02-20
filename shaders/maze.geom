#version 330 core

layout (lines) in;
layout (triangle_strip, max_vertices = 6) out;


void main()
{
    vec4 add = normalize(vec4(gl_in[0].gl_Position.y-gl_in[1].gl_Position.y,gl_in[1].gl_Position.x-gl_in[0].gl_Position.x,0,0)) * 0.02;
    
    
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