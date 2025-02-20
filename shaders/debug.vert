#version 330 core
layout(location = 0) in  vec4 aVertexPosition;

    void main() {
    gl_Position = vec4(aVertexPosition.xyz,1);
    }