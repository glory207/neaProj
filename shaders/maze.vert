#version 330 core
    precision mediump float;
    layout(location = 0) in  vec3 aVertexPosition;
    uniform vec2 campos;
    uniform float rt;
    uniform float sc;
    uniform float sc1;
    void main() {
    mat2 rot = mat2(
    sin(-rt), -cos(-rt),
    cos(-rt), sin(-rt)
    
    );
      gl_Position = vec4(rot * (aVertexPosition.xy*sc1 - vec2(campos.y,campos.x))*sc ,0.0,1.0);
    }