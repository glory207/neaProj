#version 330 core
      
      out vec4 fragColor;
      uniform sampler2D ColT;
      in vec2 texPos;


void main() {
    fragColor += vec4(texture(ColT, texPos).xyz,1.0);
}
