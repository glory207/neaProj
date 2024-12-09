#version 330 core
      
      out vec4 fragColor;
      uniform sampler2D ColT2;
      uniform samplerCube ColT;
      in vec2 texPos;


void main() {
fragColor += vec4(texture(ColT2, texPos).xyz,1.0);
  //  fragColor += vec4(texture(ColT, vec3(texPos.x,texPos.y,-1.0)).xyz,1.0) * 100.0; 
    
}
