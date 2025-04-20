# pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <math.h>
#include<glm/gtc/matrix_transform.hpp>
#include "frameBuffer.h"
#include "shader.h"

class camera
{
    
public:
    glm::vec3 pos;
    glm::vec3 rot;
    glm::mat4 rotationMatrix;
    camera(glm::vec3 pos, glm::vec3 rot);
    camera();
    void draw(GLuint shadr);
    void drawScreen();
    void updateSize(glm::vec2 sz);
    glm::mat4 matrix(float aspect);
    GframeBuffer GFB;
    FrameBuffer FB;
    FrameBuffer PFB1;
    FrameBuffer MFB;
    GLuint shader;
    GLuint VAO;
};

