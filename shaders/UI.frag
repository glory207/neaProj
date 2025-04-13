#version 330 core
      
      out vec4 fragColor;
      uniform sampler2D ColT2;
      uniform vec4 backround;
      uniform vec4 forground;
      uniform int chr;
      uniform float corne;
      in vec2 texPos;
      in vec4 fragPos;
      in vec2 fragPos2;

void main() {

    float corner = 0.03;
    float edge = 0.7;

    if(chr == -1 || chr == 85)
    {
        fragColor =backround;
        if(chr == 85)fragColor = vec4(texture(ColT2, texPos).xyz,1);
        //if( true){}else
        if(fragPos2.x < fragPos.x - fragPos.z+corner && fragPos2.y < fragPos.y - fragPos.w+corner)
        {
           if(distance(fragPos2,vec2(fragPos.x - fragPos.z+corner,fragPos.y - fragPos.w+corner))/corner>1)discard;
           if(distance(fragPos2,vec2(fragPos.x - fragPos.z+corner,fragPos.y - fragPos.w+corner))/corner>edge)fragColor = vec4(vec3(0.5f),1.0);
        }
        else if(fragPos2.x > fragPos.x + fragPos.z-corner && fragPos2.y < fragPos.y - fragPos.w+corner)
        {
           if(distance(fragPos2,vec2(fragPos.x + fragPos.z-corner,fragPos.y - fragPos.w+corner))/corner>1)discard;
           if(distance(fragPos2,vec2(fragPos.x + fragPos.z-corner,fragPos.y - fragPos.w+corner))/corner>edge)fragColor = vec4(vec3(0.5f),1.0);
        }
        else if(fragPos2.x < fragPos.x - fragPos.z+corner && fragPos2.y > fragPos.y + fragPos.w-corner)
        {
           if(distance(fragPos2,vec2(fragPos.x - fragPos.z+corner,fragPos.y + fragPos.w-corner))/corner>1)discard;
           if(distance(fragPos2,vec2(fragPos.x - fragPos.z+corner,fragPos.y + fragPos.w-corner))/corner>edge)fragColor = vec4(vec3(0.5f),1.0);
        }
        else if(fragPos2.x > fragPos.x + fragPos.z-corner && fragPos2.y > fragPos.y + fragPos.w-corner)
        {
           if(distance(fragPos2,vec2(fragPos.x + fragPos.z-corner,fragPos.y + fragPos.w-corner))/corner>1)discard;
           if(distance(fragPos2,vec2(fragPos.x + fragPos.z-corner,fragPos.y + fragPos.w-corner))/corner>edge)fragColor = vec4(vec3(0.5f),1.0);
        }
        else if((fragPos.z-abs(fragPos2.x-fragPos.x))/corner < 1.0f-edge ||(fragPos.w-abs(fragPos2.y-fragPos.y))/corner < 1.0f-edge )fragColor = vec4(vec3(0.5f),1.0);
    }else {
    
        if(texture(ColT2, (texPos+ vec2(chr,0)) * vec2(1.0/43.0,-1) ).w<0.5){
            if(backround.w>0.5)fragColor =backround;
            else discard;
        }
        else{
            if(forground.w>0.5)fragColor =forground;
            else discard;
        }
    }
    
}
