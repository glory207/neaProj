#version 330 core
layout(location = 0) in  vec4 aVertexPosition;
layout(location = 1) in  vec2 aTexturePosition;

    uniform mat4 uModelViewMatrix;
    uniform mat4 uProjectionMatrix;
    uniform mat3 normalMatrix;
    uniform vec4 textureMatrix;
    uniform vec4 NormalTextureMatrix;

    out DATA
{
	vec3 texCoord;
	vec3 texCoord2;
    mat4 projection;
} data_in;


    void main() {

    data_in.texCoord = vec3((aTexturePosition + textureMatrix.xy) * textureMatrix.zw,0);
    data_in.texCoord2 = vec3((aTexturePosition + NormalTextureMatrix.xy) * NormalTextureMatrix.zw,0);
    data_in.projection = uProjectionMatrix;
    gl_Position = uModelViewMatrix * aVertexPosition;
    }