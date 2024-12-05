#include<header/object.h>

SpObj::SpObj(glm::vec3 poss,glm::vec3 rott,glm::vec3 scaa, BufferGroup buffers){
    pos = poss;
    rot = rott;
    sca = scaa;
    buffer = buffers;
    textOff = glm::vec4(0,0,1,1);

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
    normalMatrix = glm::transpose(normalMatrix);
    glUniformMatrix3fv(glGetUniformLocation(programInfo,"aNormal"),1,GL_FALSE,glm::value_ptr(normalMatrix));
    glUniform4f(glGetUniformLocation(programInfo,"aNormal"),textOff.x,textOff.y,textOff.z,textOff.w);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffer.positions);
    glVertexAttribPointer(GL_ARRAY_BUFFER,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(glGetAttribLocation(programInfo, "aVertexPosition"));
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buffer.indices);
    glDrawElements(GL_TRIANGLES,buffer.length,GL_UNSIGNED_BYTE,0);
}