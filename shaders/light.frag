#version 330 core
      layout (location = 0) out vec4 fragColor;
      layout (location = 1) out vec4 gPos;
      layout (location = 2) out vec4 gNorm;
      layout (location = 3) out vec4 gNormF;
      uniform vec3 color;
      void main() {
        fragColor = vec4((color),1.0);
        gPos =  vec4(0.0,-1.0,0.0,1.0);
        gNorm = vec4(vec3(0.0),2.0);
        gNormF = vec4(vec3(0.0),2.0);
        
        }