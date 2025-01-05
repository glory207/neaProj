#version 330 core
layout(location = 0) in  vec4 aVertexPosition;
layout(location = 1) in  vec2 aTexturePosition;
layout(location = 2) in  vec2 txt;
layout(location = 3) in  mat4 uModelViewMatrix1;

    uniform vec4 textureMatrix;
      
out DATA
{
    vec3 pos;
    vec2 texture1;
} data_out;



    void main() {
    data_out.texture1 = txt; 
    data_out.pos = vec3((aTexturePosition + textureMatrix.xy) * textureMatrix.zw,0);
      gl_Position = uModelViewMatrix1 * aVertexPosition;
    }