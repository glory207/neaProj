#include"object.h"
#include <iostream>
#include <string>
SpObj::SpObj(glm::vec3 poss,glm::vec3 rott,glm::vec3 scaa, BufferGroup buffers, int img1, int img2){
    // the transformation data of an object
    pos = poss;
    rot = rott;
    sca = scaa;
    // the vertex data of an object
    buffer = buffers;
    // how the textures will by transformed (movement x, movement y, scale x, scale y)
    textOff = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    textOff2 = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    text1 = texture(img1);
    text2 = texture(img2);

    
}
SpObj::SpObj() {
    // the transformation data of an object
    pos = glm::vec3(0);
    rot = glm::vec3(0);
    sca = glm::vec3(1);
    // how the textures will by transformed (movement x, movement y, scale x, scale y)
    textOff = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    textOff2 = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
}
void SpObj::draw(int programInfo){
    // the matrix responsible for translating the object to world space
    glm::mat4 modelViewMatrix;
    modelViewMatrix = glm::translate(modelViewMatrix,pos);
    modelViewMatrix = glm::rotate(modelViewMatrix,rot.z,glm::vec3(0,0,1));
    modelViewMatrix = glm::rotate(modelViewMatrix,rot.y,glm::vec3(0,1,0));
    modelViewMatrix = glm::rotate(modelViewMatrix,rot.x,glm::vec3(1,0,0));
    modelViewMatrix = glm::scale(modelViewMatrix,sca);


    glm::mat3 normalMatrix = glm::mat3(modelViewMatrix);
    normalMatrix = glm::inverse(normalMatrix);
    // binds the data to the GPU memmory

    glUniformMatrix4fv(glGetUniformLocation(programInfo,"uModelViewMatrix"),1,GL_FALSE,glm::value_ptr(modelViewMatrix));
    glUniform4f(glGetUniformLocation(programInfo,"textureMatrix"),textOff.x,textOff.y,textOff.z,textOff.w);
    glUniform4f(glGetUniformLocation(programInfo,"NormalTextureMatrix"), textOff2.x, textOff2.y, textOff2.z, textOff2.w);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, text1);
    glUniform1i(glGetUniformLocation(programInfo, "uSampler1"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, text2);
    glUniform1i(glGetUniformLocation(programInfo, "uSampler2"), 1);

    glBindVertexArray(buffer.VAO);
    // draws the object to the current frame buffer
    glDrawElements(GL_TRIANGLES,buffer.length,GL_UNSIGNED_INT, 0);
    
}


UIObj::UIObj(glm::vec3 poss, glm::vec3 rott, glm::vec3 scaa) {
    obj = SpObj(poss, rott, scaa, initCubeBuffer({9}),30,30);
}