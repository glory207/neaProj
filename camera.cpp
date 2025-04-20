#include "camera.h"
camera::camera(){}
camera::camera(glm::vec3 pos, glm::vec3 rot){
    this->pos = pos;
    this->rot = rot;
    GFB = GframeBuffer(glm::vec2(512));
    MFB = FrameBuffer(glm::vec2(512));
    FB = FrameBuffer(glm::vec2(512));
    PFB1 = FrameBuffer(glm::vec2(512));

    std::vector<float> vertPos = { 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0 };
    std::vector<float> vertPos2 = { -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0 };
    std::vector<int> indices = { 2, 1, 0, 3, 2, 0 };
    GLuint BP,BP2,BI;
    glGenBuffers(1,&BP);
    glBindBuffer(GL_ARRAY_BUFFER, BP);
    glBufferData(GL_ARRAY_BUFFER, vertPos.size() * sizeof(float), vertPos.data(), GL_STATIC_DRAW);
    
    glGenBuffers(1,&BP2);
    glBindBuffer(GL_ARRAY_BUFFER, BP2);
    glBufferData(GL_ARRAY_BUFFER, vertPos2.size() * sizeof(float), vertPos2.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &BI);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BI);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO); 
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, BP);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, BP2);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BI);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    shader = initShader("shaders/cam.vert", "shaders/cam.frag");

}
void camera::draw(GLuint shadr) {
    glUniform1i(glGetUniformLocation(shadr, "ColT"), 0);
    glUniform1i(glGetUniformLocation(shadr, "PosT"), 1);
    glUniform1i(glGetUniformLocation(shadr, "NormT"), 2);
    glUniform1i(glGetUniformLocation(shadr, "NormFT"), 3);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
void camera::drawScreen() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
void camera::updateSize(glm::vec2 sz) {
    GFB.set(sz);
    FB.set(sz);
    PFB1.set(sz);
}
glm::mat4 camera::matrix(float aspect){
    float fov = (60.0f * 3.14159265358979323846f) / 180.0f;
    float zNear = 0.01f;
    float zFar = 100.0f;
    glm::mat4 projectionMatrix;
    glm::mat4 view;
    
    projectionMatrix = glm::perspective(fov,aspect,zNear,zFar);
    view = glm::rotate(view,-rot.x,glm::vec3(1,0,0));
    view = glm::rotate(view,-rot.y,glm::vec3(0,1,0));
    view = glm::rotate(view,-rot.z,glm::vec3(0,0,1));
    rotationMatrix = view;
    view = glm::translate(view,-pos);
    view = projectionMatrix * view;
    return view;
}