#version 330 core
      
layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 gPos;
layout (location = 2) out vec4 gNorm;
layout (location = 3) out vec4 gNormF;
      uniform sampler2D uSampler1;
      uniform sampler2D uSampler2;
     // uniform float time;
     // uniform int count;
      in mat3 tbn;
      in vec3 pos; 
      in vec3 pos2; 
      in vec3 po;
      in vec3 norm;
      uniform vec3 camPos;
      
      void main() {
      if(texture(uSampler2, pos2.xy ).w < 0.5 ||texture(uSampler1, pos.xy ).w < 0.5 ) discard;
      
      vec3 normal = tbn * ((texture(uSampler2, pos2.xy ).xyz * 2.0) - vec3(1));
      fragColor = vec4(texture(uSampler1, pos.xy ).xyz,1.0);
      gPos = vec4((po-camPos),1.0);
      //gPos = vec4((po/10.0f),1.0);
      gNorm = vec4(normal,1.0);
       gNormF =  vec4(norm,1.0);
      }