#version 330 core
uniform sampler2D uSampler[8];
out float fragColor;
in vec3 pos;
      in vec2 txt1;
in vec3 fpos;
uniform vec3 lpos;
in vec4 FragPos;
        
 void main() {
     int t1 = int(txt1.x+0.01);
      int t2 = int(txt1.y+0.01);
    if(texture(uSampler[t1], pos.xy ).w < 0.5 ) discard;
      

    gl_FragDepth = length(FragPos.xyz-lpos)/100.0f;
}