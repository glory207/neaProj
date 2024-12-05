#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 uProjectionMatrix[6];

out vec4 FragPos;
out vec3 pos;

in DATA
{
    vec3 pos;
} data_in[];

void main() {
	
	for(int face = 0; face < 6; ++face){
		gl_Layer = face;
		for(int i = 0; i < 3; ++i){
			FragPos = gl_in[i].gl_Position;
			pos = data_in[i].pos;
			gl_Position = uProjectionMatrix[face] * FragPos;

			EmitVertex();
		}
		EndPrimitive();
	}



}