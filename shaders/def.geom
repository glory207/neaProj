#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

    out vec3 pos;
    out vec3 norm;
    out mat3 tbn;
    out vec3 po;

in DATA
{
	vec3 texCoord;
    mat4 projection;
} data_in[];


void main()
{
    vec3 edge0 = vec3(gl_in[2].gl_Position - gl_in[0].gl_Position);
    vec3 edge1 = vec3(gl_in[1].gl_Position - gl_in[0].gl_Position);

    vec2 UV0 = vec2(data_in[2].texCoord - data_in[0].texCoord);
    vec2 UV1 = vec2(data_in[1].texCoord - data_in[0].texCoord);

    float invDet = 1.0 / (UV0.x * UV1.y - UV0.y * UV1.x);

    vec3 bitan = invDet * (UV0.x * edge1 - UV1.x * edge0);

    vec3 tan = -invDet * (UV0.y * edge1 + UV1.y * edge0);

    vec3 norma = normalize(cross(edge0, edge1));

    
    tbn = mat3(tan,bitan , norma);

    for(int i = 0; i < 3; i ++){
    norm = (norma);
    gl_Position = data_in[i].projection * (gl_in[i].gl_Position);
    po = vec3(gl_in[i].gl_Position);
    pos = data_in[i].texCoord;
    tbn = mat3(tan,-bitan , norma);
    EmitVertex();
    }

    EndPrimitive();
}