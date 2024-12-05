#version 330 core
      
      out vec4 fragColor;
      uniform sampler2D ColT;
      uniform sampler2D PosT;
      uniform sampler2D NormT;
      uniform sampler2D NormFT;
      uniform samplerCube uSamplerS;
      uniform vec3 camPos;
      in vec2 texPos;
      uniform mat4 rotcam;
      uniform vec3 lightPos;
      uniform vec3 lightCol;
      uniform int light;
      

      float getLight(vec3 posLight,vec3 posFrag,vec3 normal,vec3 normalF){
        vec3 dir = (posLight)-posFrag;
        float difVal = max(dot(normal, normalize(dir)), 0.0);
        float dist = length(dir)/0.5;
        float a = 3.0;
        float b = 0.7;
        float Falloff = 1.0 / (a * dist * dist + b * dist + 1.0);
        difVal *= Falloff;
        
       // float angle = dot(normalize((rotcam * vec4(0,0,1,0)).xyz), normalize(dir));
        float angle = dot(normalize(( vec4(0,0,1,0)).xyz), normalize(dir));
        float outerCone = 0.7;
        float innerCone = outerCone ;
        float difVal2 = clamp((angle + outerCone -1.57)/(outerCone*innerCone), 0.0, 1.0) * difVal * 5.0;
            
        bool cut = false;
        for(float k = 0.0; k < 3.0; k+= 0.5){
        if(difVal > 1.0 / (a * k * k + b * k + 1.0)) {
        cut = true;
        difVal =  1.0 / (k*k);
        break;
        }}
        if(cut == false) difVal = 0.0;
        cut = false;
        for(float k = 0.0; k < 3.0; k+= 0.5){
        if(difVal2 > 1.0 / (a * k * k + b * k + 1.0)) {
        cut = true;
        difVal2 =  1.0 / (k*k);
        break;
        }}
        if(cut == false) difVal2 = 0.0;
        difVal *= 1.0;    
        difVal2 *= 0.0;    
  
        float shadow = 0.0;
        
        float bias;
        float  closeD = 0.0;
        float currentD = length(posLight-posFrag);
        float rad = 2.0;
        for(float x = -rad; x < rad; x+= 1.0){
        for(float y = -rad; y < rad; y+= 1.0){
        for(float z = -rad; z < rad; z+= 1.0){
        closeD = texture(uSamplerS, -normalize(dir) + vec3(x,y,z) * (1.0/float(textureSize(uSamplerS,0).r))).r *100.0;
        bias = max(0.01* (1.0-dot(normalF,dir - vec3(x,y,z))), 0.001);

        if(currentD <= closeD + bias){ 
           shadow += 1.0/pow(rad*2.0,3.0);  
        }
    }}}
      if(shadow < 0.5) shadow = 0.0;
      else shadow = 1.0;

           return (difVal + difVal2) * shadow;
        }


void main() {

    if(light == 1 ){
        
            vec3 color = texture(ColT, texPos).xyz *  max(dot(texture(NormT, texPos).xyz, normalize(vec3(0.0,1.0,0.0))), 0.0) * 0.025;
            fragColor += vec4(color,1.0);
        
    }
    else if(light == 0 ){
    if(texture(PosT, texPos).y == -1.0){
            fragColor = vec4(normalize(texture(ColT, texPos).xyz),1.0);
        }else{
        vec3 color = texture(ColT, texPos).xyz * getLight(lightPos,texture(PosT, texPos).xyz+camPos,texture(NormT, texPos).xyz,texture(NormFT, texPos).xyz) * vec3(1.0);
        fragColor += vec4(color,1.0);
        //fragColor = vec4(texture(NormFT, texPos).xyz,1.0);
        }
    }

}
