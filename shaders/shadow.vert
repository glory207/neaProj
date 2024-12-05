#version 330 core
layout(location = 0) in  vec4 aVertexPosition;
layout(location = 1) in  vec2 aTexturePosition;


    uniform mat4 uModelViewMatrix;


    uniform vec4 textureMatrix;
      
out DATA
{
    vec3 pos;
} data_out;



    void main() {
    data_out.pos = vec3((aTexturePosition + textureMatrix.xy) * textureMatrix.zw,0);


      gl_Position = uModelViewMatrix * aVertexPosition;
    }