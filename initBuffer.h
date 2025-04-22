# pragma once
#include <glad/glad.h>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>

using namespace glm;


class BufferGroup
{
public:
    BufferGroup();
    BufferGroup(GLuint positions,
        GLuint texturePos,
        GLuint indices,
        GLuint length);
    BufferGroup(GLuint positions,
        GLuint indices,
        GLuint length);
    GLuint VAO;
    GLuint length;
};


int initB(std::vector<float> pointss);
int initI(std::vector<glm::mat4> pointss);
int initE(std::vector<int> pointss);
BufferGroup initMazeBuffers(std::vector<glm::vec2>  pointss);
BufferGroup initMaze2DBuffers(std::vector<glm::vec2>  pointss);
BufferGroup initCubeBuffer();
BufferGroup initSpriteBuffer();

GLuint initIndexBuffer(std::vector<int> i);
GLuint initPositionBuffer(std::vector<int>i);
GLuint initTextureBuffer(std::vector<int> i);






