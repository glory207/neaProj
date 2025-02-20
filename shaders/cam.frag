#version 330 core
      
      out vec4 fragColor;
      uniform sampler2D ColT;
      uniform sampler2D PosT;
      uniform sampler2D NormT;
      uniform sampler2D NormFT;
      uniform samplerCube uSamplerS;

      uniform vec2 LightSetings;
      uniform vec3 camPos;
      in vec2 texPos;
      uniform mat4 rotcam;
      uniform vec3 lightPos;
      uniform vec3 lightCol;
      uniform int light;
      uniform int lighC;
      uniform float ConeAngle;
      uniform float brightness;
      

      float getLight(vec3 posLight,vec3 posFrag,vec3 normal,vec3 normalF, samplerCube uSamplerSs,mat4 rot){
        vec3 dir = (posLight)-posFrag;
        float difVal = max(dot(normal, normalize(dir)), 0.0);
        float dist = length(dir);
        

        
        float shadow = 0.0;
        float bias;
        float  closeD = 0.0;
        float currentD = length(posLight-posFrag);

     closeD = texture(uSamplerSs, -normalize(dir)).r *100.0;
        bias = max(0.005* (1.0-dot(normalF,dir)), 0.005);
        if(currentD <= closeD + bias){ 
           shadow = 1.0;  
        }
        
        
        if(shadow < 0.5) return 0;
        





       // float a = 10.0;
       // float b = 0.0;
       // float Falloff = 1.0 / (a * dist * dist + b * dist + 1.0);
        float Falloff = 1.0 - pow((dist)/LightSetings.x,2.0) + 0.15;
        if(Falloff<0)Falloff=0;
        difVal *= Falloff;


        float angle = dot(normalize((inverse(rot) * vec4(0,0,1,0)).xyz), normalize(dir));
        float outerCone = ConeAngle;
        float innerCone = ConeAngle * 0.3;
        float difVal2 = clamp((angle + outerCone -1.57)/(outerCone*innerCone), 0.0, 1.0) * difVal;
        



  
      if(ConeAngle == 0) difVal2 *= 0.0;  
      else  difVal = difVal2;
        float asd = 3.0;
        if(difVal < 0.05) return 0;
        difVal = floor((difVal * asd))/asd;

        difVal += 0.5;
        // if(max(dot(normal, normalize(dir)), 0.0) > 0.2) difVal += 1.0 * (1.0- (length(dir) / LightSetings.x));
         
         difVal *= difVal * difVal * LightSetings.y; 
      // else difVal= 0.0;
       
           return (difVal);
        }


void main() {

    if(light == 1){
        
            vec3 color;
         float difVal = 0; 
         difVal += floor((max(dot(normalize(texture(NormT, texPos).xyz)*2, vec3(1,0.5,0)), 0.0) * 6.0))/6.0; 
         difVal += floor((max(dot(normalize(texture(NormT, texPos).xyz)*2, vec3(-1,0.5,0)), 0.0) * 6.0))/6.0; 
         difVal += floor((max(dot(normalize(texture(NormT, texPos).xyz)*2, vec3(0,0.5,-1)), 0.0) * 6.0))/6.0; 
         difVal += floor((max(dot(normalize(texture(NormT, texPos).xyz)*2, vec3(0,0.5,1)), 0.0) * 6.0))/6.0; 
         color = texture(ColT, texPos).xyz * brightness * floor((difVal * 3.0))/3.0;
          fragColor = vec4(color,1.0);
        if(texture(PosT, texPos).y == -1.0){
            fragColor = vec4((texture(ColT, texPos).xyz)*4,1.0);
        }
    }
    else if(light == 0 ){
    if(texture(PosT, texPos).y == -1.0){

    }
        else{
          
     vec3 color = texture(ColT, texPos).xyz * getLight(lightPos,texture(PosT, texPos).xyz+camPos,normalize(texture(NormT, texPos).xyz) * 1.5f,normalize(texture(NormFT, texPos).xyz),uSamplerS,rotcam)
      * normalize(lightCol);
     // * vec3(1.0,0.8,0.6);
       
        
        fragColor = vec4(color,1.0);
       
        }
    }

}
