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
    glm::vec3 rot;;
    camera(glm::vec3 pos, glm::vec3 rot);
    camera();
    void draw();
    void drawScreen();
    std::vector<glm::mat4> matrix(float aspect);
    GframeBuffer GFB;
    FrameBuffer FB;
    GLuint shader;
    GLuint ScreenShader;
    GLuint VAO;
};

