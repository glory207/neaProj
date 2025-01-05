# pragma once
#include <initBuffer.h>
#include<glm/gtc/matrix_transform.hpp>
#include<GL/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include "texture.h"
class SpObj
{
public:
    unsigned int VAO;
    glm::vec3 pos;
    glm::vec3 rot;
    glm::vec3 sca;
    glm::vec4 textOff;
    glm::vec4 textOff2;
    BufferGroup buffer;
    SpObj();
    SpObj(glm::vec3 poss,glm::vec3 rott,glm::vec3 scaa, BufferGroup buffers, int img1, int img2);
    GLuint text1;
    GLuint text2;
    void draw(int programInfo);
    
};

class InsObj
{
public:
    unsigned int VAO;
    glm::vec4 textOff;
    glm::vec4 textOff2;
    glm::mat4 asddsa;
    BufferGroup buffer;
    InsObj();
    InsObj(BufferGroup buffers, int img1, int img2, GLuint modelBuffer, GLuint textureBuffer);
    GLuint text1;
    GLuint text2;
    void draw(int programInfo, int count);

};


