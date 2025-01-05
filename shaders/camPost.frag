#version 330 core
      
      out vec4 fragColor;
      uniform sampler2D NormFT;
      uniform sampler2D PosT;
      uniform sampler2D ColT;
      uniform sampler2D NormT;
      in vec2 texPos;
      
void main() {
    float dist = length(texture(PosT,texPos).xyz);
    float dst = 2.0;
    if(dist > dst)fragColor = vec4(mix(vec3(0.0),texture(ColT,texPos).xyz,pow(2.0,-pow((dst -dist)/2.0,2.0))),1.0);
    else fragColor = texture(ColT,texPos);
}
