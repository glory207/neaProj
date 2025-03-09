#version 330 core
      layout (location = 0) out vec4 fragColor;
      layout (location = 1) out vec4 gPos;
      layout (location = 2) out vec4 gNorm;
      layout (location = 3) out vec4 gNormF;
      uniform sampler2D uSampler1;
      uniform sampler2D uSampler2;
      uniform vec3 color;
      uniform float tim;
      in vec3 pos; 
      void main() {
      if(texture(uSampler1, vec2((pos.x + tim)/5.0f,pos.y) ).w < 0.5 ) discard;
      
        fragColor = vec4(texture(uSampler1,vec2((pos.x + tim)/5.0f,pos.y) ).xyz,1.0);
        gPos =  vec4(-1,-1,-1,1.0);
        gNorm = vec4(vec3(0.0),2.0);
        gNormF = vec4(vec3(0.0),2.0);
        
        }