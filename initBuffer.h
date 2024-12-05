# pragma once
#include <glad/glad.h>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
class BufferGroup
{
public:
    BufferGroup();
    BufferGroup(GLuint positions,
        GLuint texturePos,
        GLuint normal,
        GLuint tangent,
        GLuint bitangent,
        GLuint indices,
        GLuint length);
    GLuint positions;
    GLuint texturePos;
    GLuint normal;
    GLuint tangent;
    GLuint bitangent;
    GLuint indices;
    GLuint length;
};

int initB(std::vector<float> pointss);
int initE(std::vector<int> pointss);
BufferGroup initBuffers(std::vector<glm::vec2>  pointss);
BufferGroup initCubeBuffer(std::vector<int> i);

GLuint initIndexBuffer2(std::vector<int> i);
GLuint initNormalBuffer(std::vector<int> i);
GLuint initbiTangentBuffer(std::vector<int>i);
GLuint initTangentBuffer(std::vector<int> i);
GLuint initPositionBuffer2(std::vector<int>i);
GLuint initTextureBuffer(std::vector<int> i);




