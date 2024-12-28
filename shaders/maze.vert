#version 300 es
    precision mediump float;
    layout(location = 0) in  vec3 aVertexPosition;
    uniform vec2 campos;
    uniform float rt;
    void main() {
    mat2 rot = mat2(
    sin(-rt), -cos(-rt),
    cos(-rt), sin(-rt)
    
    );
      gl_Position = vec4(rot * (aVertexPosition.xy - vec2(campos.y,campos.x))*0.25 ,0.0,1.0);
    }