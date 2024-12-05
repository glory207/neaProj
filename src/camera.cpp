#include "header/camera.h"

camera::camera(glm::vec3 pos, glm::vec3 rot){
    this->pos = pos;
    this->rot = rot;
}

std::vector<glm::mat4> camera::matrix(float aspect){
    float fov = (45.0f * 3.14159265358979323846f) / 180.0f;
    float zNear = 0.01f;
    float zFar = 100.0f;
    glm::mat4 projectionMatrix;
    glm::mat4 view;
    glm::mat4 rotationMatrix;
    projectionMatrix = glm::perspective(fov,aspect,zNear,zFar);
    view = glm::rotate(view,-rot.x,glm::vec3(1,0,0));
    view = glm::rotate(view,-rot.y,glm::vec3(0,1,0));
    view = glm::rotate(view,-rot.z,glm::vec3(0,0,1));
    rotationMatrix = view;
    view = projectionMatrix * view;
    return {view,rotationMatrix};
}