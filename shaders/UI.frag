#version 330 core
      
      out vec4 fragColor;
      uniform sampler2D ColT2;
      uniform vec4 backround;
      uniform vec4 forground;
      uniform vec3 edgeC;
      uniform int chr;
      uniform float corne;
      in vec2 texPos;
      in vec4 cubeTransform;
      in vec2 fragPos;

void main() {

    float corner = 0.03;
    float edge = 0.8;

    if(chr == -1 || chr == 85)
    {
        fragColor =backround;
        if(chr == 85)fragColor = vec4(texture(ColT2, texPos).xyz,1);

        // if the pixel is in the top corner
        if(fragPos.x < cubeTransform.x - cubeTransform.z+corner && fragPos.y < cubeTransform.y - cubeTransform.w+corner){
            // if its outside the curve
            if(distance(fragPos,vec2(cubeTransform.x - cubeTransform.z+corner,cubeTransform.y - cubeTransform.w+corner))/corner>1)
                discard;
            // if its just within the edge
            if(distance(fragPos,vec2(cubeTransform.x - cubeTransform.z+corner,cubeTransform.y - cubeTransform.w+corner))/corner>edge)
                fragColor = vec4(edgeC,1.0);
        }
        else if(fragPos.x > cubeTransform.x + cubeTransform.z-corner && fragPos.y < cubeTransform.y - cubeTransform.w+corner)
        {
           if(distance(fragPos,vec2(cubeTransform.x + cubeTransform.z-corner,cubeTransform.y - cubeTransform.w+corner))/corner>1)discard;
           if(distance(fragPos,vec2(cubeTransform.x + cubeTransform.z-corner,cubeTransform.y - cubeTransform.w+corner))/corner>edge)fragColor = vec4(edgeC,1.0);
        }
        else if(fragPos.x < cubeTransform.x - cubeTransform.z+corner && fragPos.y > cubeTransform.y + cubeTransform.w-corner)
        {
           if(distance(fragPos,vec2(cubeTransform.x - cubeTransform.z+corner,cubeTransform.y + cubeTransform.w-corner))/corner>1)discard;
           if(distance(fragPos,vec2(cubeTransform.x - cubeTransform.z+corner,cubeTransform.y + cubeTransform.w-corner))/corner>edge)fragColor = vec4(edgeC,1.0);
        }
        else if(fragPos.x > cubeTransform.x + cubeTransform.z-corner && fragPos.y > cubeTransform.y + cubeTransform.w-corner)
        {
           if(distance(fragPos,vec2(cubeTransform.x + cubeTransform.z-corner,cubeTransform.y + cubeTransform.w-corner))/corner>1)discard;
           if(distance(fragPos,vec2(cubeTransform.x + cubeTransform.z-corner,cubeTransform.y + cubeTransform.w-corner))/corner>edge)fragColor = vec4(edgeC,1.0);
        }
        else if((cubeTransform.z-abs(fragPos.x-cubeTransform.x))/corner < 1.0f-edge ||(cubeTransform.w-abs(fragPos.y-cubeTransform.y))/corner < 1.0f-edge )fragColor = vec4(edgeC,1.0);
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
