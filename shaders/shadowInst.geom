#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 uProjectionMatrix[6];

out vec4 FragPos;
out vec3 pos;

  out vec2 txt1;
in DATA
{
    vec3 pos;
    vec2 texture1;
} data_in[];

void main() {
	
	for(int face = 0; face < 6; ++face){
		gl_Layer = face;
		for(int i = 0; i < 3; ++i){
		txt1 = data_in[i].texture1;
			FragPos = gl_in[i].gl_Position;
			pos = data_in[i].pos;
			gl_Position = uProjectionMatrix[face] * FragPos;

			EmitVertex();
		}
		EndPrimitive();
	}



}