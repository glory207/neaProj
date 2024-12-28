#version 330 core
    layout(location = 0) in  vec2 aVertexPosition;
    out vec2 texPos;
    void main() {
    texPos = aVertexPosition;
      gl_Position = vec4(aVertexPosition * 2.0 - vec2(1.0),0.0,1.0);
    }