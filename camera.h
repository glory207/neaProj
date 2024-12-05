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
    std::vector<glm::mat4> matrix(float aspect);
    GframeBuffer GFB;
    GLuint shader;
    GLuint VAO;
};

