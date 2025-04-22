#version 330 core
    precision mediump float;
    layout(location = 0) in  vec3 aVertexPosition;
    uniform vec3 campos;
    uniform float rt;
    uniform float sc;
    uniform float sc1;
    uniform float thk;

    
out DATA
{
    vec3 col;
    float scale;
    float scale2;
    mat2 r;
} data_out;

    void main() {
    mat2 rot = mat2(
    sin(-rt), -cos(-rt),
    cos(-rt), sin(-rt)
    
    );

    data_out.scale = thk;
    data_out.scale2 = sc1 * sc;
    data_out.r = rot;

    
    if(aVertexPosition.z + campos.z> 5.01f){ data_out.col = vec3(1,0,0);}
    else if(aVertexPosition.z  + campos.z> 0.01f){
        data_out.scale *= 6.5f;
        data_out.scale2 *= 2.5f;
        data_out.col = vec3(1,1,0);}
    else if(aVertexPosition.z + campos.z < -0.01f){
        data_out.scale *= 6.5f;
        data_out.scale2 *= 2.5f;
        data_out.col = vec3(1,1,1);}
    else data_out.col = vec3(0.5f, 0.3f, 0.2f);

      gl_Position = vec4(rot * (aVertexPosition.xy - vec2(campos.y,campos.x))*sc ,0.0,1.0);
    }