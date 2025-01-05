#version 330 core
layout(location = 0) in  vec4 aVertexPosition;
layout(location = 1) in  vec2 aTexturePosition;
layout(location = 2) in  vec2 txt;
layout(location = 3) in  mat4 uModelViewMatrix1;

    uniform mat4 uProjectionMatrix;
    uniform mat3 normalMatrix;
    uniform vec4 textureMatrix;
    uniform vec4 textureMatrix2;

    out DATA
{
	vec3 texCoord;
	vec3 texCoord2;
    mat4 projection;
    vec2 texture1;
} data_in;


    void main() {
    
    data_in.texCoord = vec3((aTexturePosition + textureMatrix.xy) * textureMatrix.zw,0);
    data_in.texCoord2 = vec3((aTexturePosition + textureMatrix2.xy) * textureMatrix2.zw,0);
    data_in.texture1 = txt; 
    data_in.projection = uProjectionMatrix;
   gl_Position =  uModelViewMatrix1 * aVertexPosition;
 
}