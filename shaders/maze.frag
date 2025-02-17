#version 330 core
      layout (location = 0) out vec4 fragColor;
      
      uniform vec3 col;
      void main() {
        fragColor = vec4(col,1.0);
        
        }