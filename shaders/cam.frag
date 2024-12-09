#version 330 core
      
      out vec4 fragColor;
      uniform sampler2D ColT;
      uniform sampler2D PosT;
      uniform sampler2D NormT;
      uniform sampler2D NormFT;
      uniform samplerCube uSamplerS[11];

      uniform vec2 LightSetings;
      uniform vec3 camPos;
      in vec2 texPos;
      uniform mat4 rotcam[11];
      uniform vec3 lightPos[11];
      uniform vec3 lightCol;
      uniform int light;
      uniform int lighC;
      

      float getLight(vec3 posLight,vec3 posFrag,vec3 normal,vec3 normalF, samplerCube uSamplerSs,mat4 rot){
        vec3 dir = (posLight)-posFrag;
        float difVal = max(dot(normal, normalize(dir)), 0.0);
        float dist = length(dir);
        float a = 1.0;
        float Falloff = 1.0 - pow(dist/LightSetings.x,0.5);
        if(Falloff<0)Falloff=0;
        difVal *= Falloff;


        float angle = dot(normalize((rot * vec4(0,0,1,0)).xyz), normalize(dir));
        float outerCone = 0.7;
        float innerCone = outerCone ;
        float difVal2 = clamp((angle + outerCone -1.57)/(outerCone*innerCone), 0.0, 1.0) * difVal * 5.0;
        



  
        difVal2 *= 0.0;  
        difVal += difVal2;
       // if(difVal>0.125){
        
       // 
       // }
       // else difVal = 0;
       float difVal3 = 0;
        if(length(dir) < LightSetings.x) difVal3 = 1.0;
        float asd = 5.0;
        
         difVal *= LightSetings.y;  
         if(length(dir) < LightSetings.x) difVal += 0.1;
      // else difVal= 0.0;
        float shadow = 0.0;
        
        float bias;
        float  closeD = 0.0;
        float currentD = length(posLight-posFrag);
        float rad = 1.0;
        for(float x = -rad; x < rad; x+= 1.0){
        for(float y = -rad; y < rad; y+= 1.0){
        for(float z = -rad; z < rad; z+= 1.0){
        closeD = texture(uSamplerSs, -normalize(dir) + vec3(x,y,z) * (1.0/float(textureSize(uSamplerSs,0).r))).r *100.0;
        bias = max(0.05* (1.0-dot(normalF,dir - vec3(x,y,z))), 0.05);

        if(currentD <= closeD + bias){ 
           shadow += 1.0/pow(rad*2.0,3.0);  
        }
    }}}
    shadow =  floor(shadow*4.0)/4.0;
           return (difVal * shadow) ;
        }


void main() {

    if(light == 1){
        
            vec3 color;
           color = texture(ColT, texPos).xyz * floor( max(dot(texture(NormT, texPos).xyz, normalize(vec3(0.0,1.0,0.0))), 0.0) * 3.0)/3.0 * 0.1;
            
            fragColor = vec4(color,1.0);
        
    }
    else if(light == 0 ){
    if(texture(PosT, texPos).y == -1.0){
            fragColor = vec4(normalize(texture(ColT, texPos).xyz),1.0);
    }
        else{
         vec3 color ;
        for(int i = 0; i < lighC; i++)
        {
       
       color+= texture(ColT, texPos).xyz * getLight(lightPos[i],texture(PosT, texPos).xyz+camPos,texture(NormT, texPos).xyz,texture(NormFT, texPos).xyz,uSamplerS[i],rotcam[i]) * vec3(1.0);
       

        }
        
        fragColor += vec4(color,1.0);
       
        }
    }

}
