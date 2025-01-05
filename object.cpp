#include"object.h"
#include <iostream>
#include <string>
SpObj::SpObj(glm::vec3 poss,glm::vec3 rott,glm::vec3 scaa, BufferGroup buffers, int img1, int img2){
    pos = poss;
    rot = rott;
    sca = scaa;
    buffer = buffers;
    textOff = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    text1 = texture(img1,false);
    text2 = texture(img2,false);

    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.positions);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.texturePos);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.indices);
    

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
SpObj::SpObj() {
    pos = glm::vec3(0);
    rot = glm::vec3(0);
    sca = glm::vec3(1);
    textOff = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

}

void SpObj::draw(int programInfo){
    glm::mat4 modelViewMatrix;
    modelViewMatrix = glm::translate(modelViewMatrix,pos);
    modelViewMatrix = glm::rotate(modelViewMatrix,rot.z,glm::vec3(0,0,1));
    modelViewMatrix = glm::rotate(modelViewMatrix,rot.y,glm::vec3(0,1,0));
    modelViewMatrix = glm::rotate(modelViewMatrix,rot.x,glm::vec3(1,0,0));
    modelViewMatrix = glm::scale(modelViewMatrix,sca);

    glUniformMatrix4fv(glGetUniformLocation(programInfo,"uModelViewMatrix"),1,GL_FALSE,glm::value_ptr(modelViewMatrix));

    glm::mat3 normalMatrix = glm::mat3(modelViewMatrix);
    normalMatrix = glm::inverse(normalMatrix);
    glUniform4f(glGetUniformLocation(programInfo,"textureMatrix"),textOff.x,textOff.y,textOff.z,textOff.w);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, text1);
    glUniform1i(glGetUniformLocation(programInfo, "uSampler1"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, text2);
    glUniform1i(glGetUniformLocation(programInfo, "uSampler2"), 1);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,buffer.length,GL_UNSIGNED_INT, 0);
    
}
InsObj::InsObj(BufferGroup buffers, int img1, int img2, GLuint modelBuffer, GLuint textureBuffer){
   
    buffer = buffers;
    textOff = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    textOff2 = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    text1 = texture(img1,false);
    text2 = texture(img2,false);

    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.positions);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.texturePos);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.indices);


    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glVertexAttribDivisor(2, 1);

    glBindBuffer(GL_ARRAY_BUFFER, modelBuffer);
    
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)0);
    glVertexAttribDivisor(3, 1);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(1 * sizeof(glm::vec4)));
    glVertexAttribDivisor(4, 1);

    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(2 * sizeof(glm::vec4)));
    glVertexAttribDivisor(5, 1);
                                                                                                                                  
    glEnableVertexAttribArray(6); 
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(3 * sizeof(glm::vec4)));
    glVertexAttribDivisor(6, 1);  



    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
InsObj::InsObj() {
    textOff = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

}

void InsObj::draw(int programInfo,int count){

    glUniform4f(glGetUniformLocation(programInfo,"textureMatrix"),textOff.x,textOff.y,textOff.z,textOff.w);
    glUniform4f(glGetUniformLocation(programInfo,"textureMatrix2"),textOff2.x,textOff2.y,textOff2.z,textOff2.w);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, text1);
    glUniform1i(glGetUniformLocation(programInfo, "uSampler1"), 0); 

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, text2);
    glUniform1i(glGetUniformLocation(programInfo, "uSampler2"), 1); 
   // for (int i = 0; i < 23; i++)
   // {
   //     std::string ii = "uSamplerS[" + std::to_string(i) + "]";
   //     glUniformHandleui64ARB(glGetUniformLocation(programInfo, ii.c_str()), texture(i,true));
   // }

    //std::string ii = "uSamplerS[" + std::to_string(i) + "]";

    glUniformHandleui64ARB(glGetUniformLocation(programInfo, "uSampler[0]"), GetTexturesHandle(12));
    glUniformHandleui64ARB(glGetUniformLocation(programInfo, "uSampler[1]"), GetTexturesHandle(15));
    glUniformHandleui64ARB(glGetUniformLocation(programInfo, "uSampler[2]"), GetTexturesHandle(16));
    glUniformHandleui64ARB(glGetUniformLocation(programInfo, "uSampler[3]"), GetTexturesHandle(23));
    glUniformHandleui64ARB(glGetUniformLocation(programInfo, "uSampler[4]"), GetTexturesHandle(24));
    glUniformHandleui64ARB(glGetUniformLocation(programInfo, "uSampler[5]"), GetTexturesHandle(25));
    glUniformHandleui64ARB(glGetUniformLocation(programInfo, "uSampler[6]"), GetTexturesHandle(26));
    glUniformHandleui64ARB(glGetUniformLocation(programInfo, "uSampler[7]"), GetTexturesHandle(13));

    glBindVertexArray(VAO);
    glDrawElementsInstanced(GL_TRIANGLES,buffer.length,GL_UNSIGNED_INT, 0, count);
    
}