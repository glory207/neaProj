#version 330 core
    precision mediump float;
    layout(location = 0) in  vec2 aVertexPosition;
    uniform vec2 screen;
    out vec2 texPos;
    void main() {
    texPos = aVertexPosition;
    float widtht = 0.3;
    float x = aVertexPosition.x *widtht  ;
    float y = ((screen.x * widtht)/screen.y);
      gl_Position = vec4(vec2((x ) -1.0,(y * aVertexPosition.y)-1.0),0.0,1.0);
    }