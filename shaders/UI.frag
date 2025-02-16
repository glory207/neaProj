#version 330 core
      
      out vec4 fragColor;
      uniform sampler2D ColT2;
      uniform vec4 backround;
      uniform vec4 forground;
      uniform int chr;
      uniform float corner;
      in vec2 texPos;
      in vec4 fragPos;
      in vec2 fragPos2;

void main() {
if(texture(ColT2, (texPos+ vec2(chr,0)) * vec2(1.0/43.0,-1.0) ).w<0.5){
if(backround.w>0.5)fragColor =backround;
else discard;
}
else{
if(forground.w>0.5)fragColor =forground;
else discard;
}
if(chr == -1)
{
    if(fragPos2.x < fragPos.x - fragPos.z+corner && fragPos2.y < fragPos.y - fragPos.w+corner)
    {
       if(distance(fragPos2,vec2(fragPos.x - fragPos.z+corner,fragPos.y - fragPos.w+corner))/corner>1)discard;
    }
    else if(fragPos2.x > fragPos.x + fragPos.z-corner && fragPos2.y < fragPos.y - fragPos.w+corner)
    {
       if(distance(fragPos2,vec2(fragPos.x + fragPos.z-corner,fragPos.y - fragPos.w+corner))/corner>1)discard;
    }
    else if(fragPos2.x < fragPos.x - fragPos.z+corner && fragPos2.y > fragPos.y + fragPos.w-corner)
    {
       if(distance(fragPos2,vec2(fragPos.x - fragPos.z+corner,fragPos.y + fragPos.w-corner))/corner>1)discard;
    }
    else if(fragPos2.x > fragPos.x + fragPos.z-corner && fragPos2.y > fragPos.y + fragPos.w-corner)
    {
       if(distance(fragPos2,vec2(fragPos.x + fragPos.z-corner,fragPos.y + fragPos.w-corner))/corner>1)discard;
    }
}
}
