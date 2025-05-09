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
      uniform vec3 rotcam;
      uniform vec3 lightPos;
      uniform vec3 lightCol;
      uniform int light;
      uniform int lighC;
      uniform float ConeAngle;
      uniform float brightness;
      uniform bool gradient;
      

      float getLight(vec3 posLight,vec3 posFrag,vec3 normal,vec3 normalF, samplerCube uSamplerSs){
        vec3 dir = (posLight)-posFrag;
        float dist = length(dir);

        // only shade if facing the light
        float difVal = max(dot(normal, normalize(dir)), 0.0);

        float shadow = 0.0;
        float bias;
        float  closeD = 0.0;
        float currentD = length(posLight-posFrag);

        // if covered by onother object dont light it
        closeD = texture(uSamplerSs, -normalize(dir)).r *100.0;
        bias = max(0.005* (1.0-dot(normalF,dir)), 0.005);
        if(currentD <= closeD + bias){ 
           shadow = 1.0;  
        }
        
        if(shadow < 0.5) return 0;
        
        // reduces the intencity of the light using a quadratic function
        // at distance LightSetings.x it will be 0
        float Falloff = 1.0 - pow((dist)/LightSetings.x,2.0) + 0.15;
        if(Falloff<0)Falloff=0;
        difVal *= Falloff;

        // gets the intencity of the light within the cone
        float angle = dot(normalize(rotcam), normalize(dir));
        float outerCone = ConeAngle;
        float innerCone = ConeAngle * 0.3;
        float difVal2 = clamp((angle + outerCone -1.57)/(outerCone*innerCone), 0.0, 1.0) * difVal;
        
        // removes the cone if the angle is 0
        if(ConeAngle == 0) difVal2 *= 0.0; 
        else difVal = difVal2;

        // clams the values of light for a nice visual effect
        float steps = 4.0;
        if(difVal < 0.05) return 0;
        difVal = floor((difVal * steps))/steps;
        difVal += 0.5;
        difVal = difVal * difVal * difVal * LightSetings.y; 
       
        return (difVal);
        }


void main() {
// preforms the ambient lighting giving enerything a base glow
    if(light == 1){
        vec3 color;
        float difVal = 8.0f; 
        color = texture(ColT, texPos).xyz * brightness * difVal;
        fragColor = vec4(color,1.0);
        // outputs the exact colors of objects unaffected by light e.g fire
        if(texture(PosT, texPos).xyz == vec3(-1.0)){
            fragColor = vec4((texture(ColT, texPos).xyz)*4,1.0);
        }

    }
    else if(light == 0 ){
  // shades the objects that are affected by light
            if(texture(PosT, texPos).xyz != vec3(-1.0)){

            vec3 color = texture(ColT, texPos).xyz * lightCol *
                         getLight(
                                 lightPos,
                                 texture(PosT, texPos).xyz+camPos,
                                 normalize(texture(NormT, texPos).xyz) * 1.5f,
                                 normalize(texture(NormFT, texPos).xyz),
                                 uSamplerS);

            fragColor = vec4(color,1.0);
        }
    }

}
