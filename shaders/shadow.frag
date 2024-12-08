#version 330 core
uniform sampler2D uSampler1;
out float fragColor;
in vec3 pos;
in vec3 fpos;
uniform vec3 lpos;
in vec4 FragPos;
        
 void main() {
     if(texture(uSampler1, pos.xy ).w < 0.5 ) discard;
      

    gl_FragDepth = length(FragPos.xyz-lpos)/100.0f;
}