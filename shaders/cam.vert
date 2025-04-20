#version 330 core
    layout(location = 0) in  vec2 aTexturePosition;
    layout(location = 1) in  vec2 aVertexPosition;
    
    uniform vec4 textureMatrix;
    out vec2 texPos;
    out vec2 fragPos;
    out vec4 cubeTransform;
    void main() {
    texPos = aTexturePosition;
    cubeTransform = textureMatrix;
    fragPos = (aVertexPosition * textureMatrix.zw + textureMatrix.xy);
      gl_Position = vec4(aVertexPosition * textureMatrix.zw + textureMatrix.xy,1.0,1.0);
    }