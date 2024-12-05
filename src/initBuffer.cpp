#include "header/initBuffer.h"

BufferGroup initBuffers(std::vector<glm::vec2> pointss){
    std::vector<float> positions;
    std::vector<float> texturePos;
    std::vector<float> normals;
    std::vector<float> tangent;
    std::vector<float> bitangent;
    std::vector<float> indices;
    float size = 0.5f;
    for (int i = 0; i < pointss.size()-2; i+=2){
        float dis = glm::sqrt(glm::pow(pointss[i].x - pointss[i+1].x,2.0f)+glm::pow(pointss[i].y - pointss[i+1].y,2.0f));

        glm::vec3 edge0 = glm::vec3(
            pointss[i+1].x - pointss[i].x,
            0.0f, 
            pointss[i+1].y - pointss[i].y
        );
        glm::vec3 edge1 = glm::vec3(
            0.0f,
            size, 
            0.0f
        );
        // start


        
    glm::vec2 UV0 = glm::vec2(
      dis,
      0
    );
    
    glm::vec2 UV1 = glm::vec2(
      0,
      -size
    );
    float invDet = 1.0 / (UV0.x * UV1.y - UV0.y * UV1.x);
    glm::vec3 tan = glm::vec3(
      invDet * (UV1.y * edge0.x + UV0.y * edge1.x),
      invDet * (UV1.y * edge0.y + UV0.y * edge1.y),
      invDet * (UV1.y * edge0.z + UV0.y * edge1.z)
    );
    glm::vec3 bitan = glm::vec3(
      invDet * (-UV1.x * edge0.x + UV0.x * edge1.x),
      invDet * (-UV1.x * edge0.y + UV0.x * edge1.y),
      invDet * (-UV1.x * edge0.z + UV0.x * edge1.z)
    );
    edge0 = glm::cross(edge1,edge0);
    for (int k = 0; k < 4; k++)
    {
        tangent.push_back(tan.x);
        tangent.push_back(tan.y);
        tangent.push_back(tan.z);
        bitangent.push_back(tan.x);
        bitangent.push_back(tan.y);
        bitangent.push_back(tan.z);

        normals.push_back(tan.x);
        normals.push_back(tan.y);
        normals.push_back(tan.z);
    }
    

    positions.push_back(pointss[i].x);
    positions.push_back(0);
    positions.push_back(pointss[i].y);
    texturePos.push_back(0);
    texturePos.push_back(size);
    
    positions.push_back(pointss[i+1].x);
    positions.push_back(0);
    positions.push_back(pointss[i+1].y);
    texturePos.push_back(dis);
    texturePos.push_back(size);
    

    positions.push_back(pointss[i].x);
    positions.push_back(size);
    positions.push_back(pointss[i].y);
    texturePos.push_back(0);
    texturePos.push_back(0);
    

    positions.push_back(pointss[i+1].x );
    positions.push_back(size);
    positions.push_back(pointss[i+1].y );
    texturePos.push_back(dis);
    texturePos.push_back(0);

    indices.push_back((i*2));
    indices.push_back((i*2)+1);
    indices.push_back((i*2)+2);
    indices.push_back((i*2)+1);
    indices.push_back((i*2)+3);
    indices.push_back((i*2)+2);
    }
    BufferGroup bg;
bg.positions = initB(positions);
bg.texturePos = initB(texturePos);
bg.normal = initB(normals);
bg.bitangent = initB(bitangent);
bg.tangent = initB(tangent);
bg.bitangent = initB(bitangent);
bg.indices = initB(indices);
return  bg;
}
int initB(std::vector<float> pointss){
    
    unsigned int B;
    glGenBuffers(1,&B);
    glBindBuffer(GL_ARRAY_BUFFER, B);
    glBufferData(GL_ARRAY_BUFFER, pointss.size() * sizeof(float),pointss.data(),GL_STATIC_DRAW);
}

int initE(std::vector<int> pointss){
    unsigned int B;
    glGenBuffers(1,&B);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, B);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, pointss.size() * sizeof(int),pointss.data(),GL_STATIC_DRAW);
}