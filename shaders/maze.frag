#version 330 core
      layout (location = 0) out vec4 fragColor;
      
      in vec3 col2;
      uniform vec3 col;
      void main() {
      vec4 color = vec4(col2,1.0);
      
        fragColor = color;
        
        }