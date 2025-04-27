#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

    out vec3 pos;
    out vec3 pos2;
    out vec3 norm;
    out mat3 tbn;
    out vec3 po;
      uniform vec3 camPos;

in DATA
{
	vec3 texCoord;
	vec3 texCoord2;
    mat4 projection;
} data_in[];


void main()
{
// calculate the normal
    vec3 edge0 = vec3(gl_in[2].gl_Position - gl_in[0].gl_Position);
    vec3 edge1 = vec3(gl_in[1].gl_Position - gl_in[0].gl_Position);

    vec2 UV0 = vec2(data_in[2].texCoord2 - data_in[0].texCoord2);
    vec2 UV1 = vec2(data_in[1].texCoord2 - data_in[0].texCoord2);

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
    pos2 = data_in[i].texCoord2;
    if(dot(vec3(gl_in[i].gl_Position)-camPos,norma) < 0)
    tbn = mat3(tan,-bitan , norma);
    else
    tbn = mat3(tan,-bitan , -norma);

    EmitVertex();
    }

    EndPrimitive();
}