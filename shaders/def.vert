#version 330 core
layout(location = 0) in  vec4 aVertexPosition;
layout(location = 1) in  vec2 aTexturePosition;
layout(location = 2) in  vec3 aNormal;
layout(location = 3) in  vec3 aTangent;
layout(location = 4) in  vec3 aBitangent;

    uniform mat4 uModelViewMatrix;
    uniform mat4 uProjectionMatrix;
    uniform mat3 normalMatrix;
    uniform vec4 textureMatrix;
    out vec3 pos;
    out vec3 norm;
    out mat3 tbn;
    out vec3 po;


    void main() {
    pos = vec3((aTexturePosition + textureMatrix.xy) * textureMatrix.zw,0);
   // pos = vec3(aTexturePosition,0);
    vec3 norma = normalize(normalMatrix * aNormal);
    vec3 tan = normalize(normalMatrix * aTangent);
    vec3 bitan = normalize(normalMatrix * aBitangent);
    norm = norma;
    tbn = mat3(tan,-bitan , norma);
    po = (uModelViewMatrix *aVertexPosition).xyz;
      gl_Position = uProjectionMatrix * uModelViewMatrix * aVertexPosition;
    }