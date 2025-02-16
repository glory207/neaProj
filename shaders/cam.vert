#version 330 core
    layout(location = 0) in  vec2 aTexturePosition;
    layout(location = 1) in  vec2 aVertexPosition;
    
    uniform vec4 textureMatrix;
    out vec2 texPos;
    out vec4 fragPos;
    out vec2 fragPos2;
    void main() {
    texPos = aTexturePosition;
    fragPos = textureMatrix;
    fragPos2 = (aVertexPosition * textureMatrix.zw + textureMatrix.xy);
      gl_Position = vec4(aVertexPosition * textureMatrix.zw + textureMatrix.xy,1.0,1.0);
    }