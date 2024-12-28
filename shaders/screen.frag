#version 330 core
      
      out vec4 fragColor;
      uniform sampler2D ColT2;
      in vec2 texPos;


void main() {
fragColor += vec4(texture(ColT2, texPos).xyz,1.0);
}
