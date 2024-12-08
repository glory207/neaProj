#include "initBuffer.h"
#include <iostream>

BufferGroup initBuffers(std::vector<glm::vec2> pointss) {
    std::vector<float> positions;
    std::vector<float> texturePos;
    std::vector<int> indices;
    float size = 0.5f;
    for (int i = 0; i < pointss.size() - 2; i += 2) {
        float dis = glm::sqrt(glm::pow(pointss[i].x - pointss[i + 1].x, 2.0f) + glm::pow(pointss[i].y - pointss[i + 1].y, 2.0f));

        glm::vec3 edge0 = glm::vec3(
            pointss[i + 1].x - pointss[i].x,
            0.0f,
            pointss[i + 1].y - pointss[i].y
        );
        glm::vec3 edge1 = glm::vec3(
            0.0f,
            size,
            0.0f
        );
        // start



        glm::vec2 UV0 = glm::vec2(
            dis,
            0.0f
        );

        glm::vec2 UV1 = glm::vec2(
            0.0f,
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
        edge0 = glm::cross(edge1, edge0);
        



        positions.push_back(pointss[i + 1].x);
        positions.push_back(0);
        positions.push_back(pointss[i + 1].y);
        texturePos.push_back(dis);
        texturePos.push_back(size);

        positions.push_back(pointss[i].x);
        positions.push_back(0);
        positions.push_back(pointss[i].y);
        texturePos.push_back(0);
        texturePos.push_back(size);


        positions.push_back(pointss[i].x);
        positions.push_back(size);
        positions.push_back(pointss[i].y);
        texturePos.push_back(0);
        texturePos.push_back(0);

        positions.push_back(pointss[i + 1].x);
        positions.push_back(size);
        positions.push_back(pointss[i + 1].y);
        texturePos.push_back(dis);
        texturePos.push_back(0);




        indices.push_back((i * 2) + 1);
        indices.push_back((i * 2));
        indices.push_back((i * 2) + 2);
       
        indices.push_back((i * 2) + 3);
        indices.push_back((i * 2) + 2);
        indices.push_back((i * 2) );

    }

    {

        int i = ((pointss.size() - 4) * 2) + 4;
        float dis = (pointss[pointss.size() - 1].x - pointss[pointss.size() - 2].x);


        glm::vec3 edge0 = glm::vec3(
            pointss[pointss.size() - 2].x - pointss[pointss.size() - 1].x,
            0,
            pointss[pointss.size() - 1].y - pointss[pointss.size() - 1].y
        );

        glm::vec3 edge1 = glm::vec3(
            pointss[pointss.size() - 2].x - pointss[pointss.size() - 1].x,
            0,
            pointss[pointss.size() - 2].y - pointss[pointss.size() - 1].y
        );

        glm::vec2 UV0 = glm::vec2(
            dis - 0,
            0
        );

        glm::vec2 UV1 = glm::vec2(
            dis - 0,
            dis - 0
        );
        float invDet = 1.0f / (UV0.x * UV1.y - UV0.y * UV1.x);
        glm::vec3 tan = glm::vec3(
            invDet * (UV1.y * edge0.x + UV0.y * edge1.x),
            invDet * (UV1.y * edge0.y + UV0.y * edge1.y),
            invDet * (UV1.y * edge0.z + UV0.y * edge1.z)
        );
        glm::vec3 bitan = glm::vec3(
            invDet * (-UV1.y * edge0.x + UV0.x * edge1.x),
            invDet * (-UV1.y * edge0.y + UV0.x * edge1.y),
            invDet * (-UV1.y * edge0.z + UV0.x * edge1.z)
        );

        positions.push_back(pointss[pointss.size() - 1].x);
        positions.push_back(0);
        positions.push_back(pointss[pointss.size() - 1].y);
        texturePos.push_back(0);
        texturePos.push_back(0);



        positions.push_back(pointss[pointss.size() - 2].x);
        positions.push_back(0);
        positions.push_back(pointss[pointss.size() - 1].y);
        texturePos.push_back(dis);
        texturePos.push_back(0);

        positions.push_back(pointss[pointss.size() - 2].x);
        positions.push_back(0);
        positions.push_back(pointss[pointss.size() - 2].y);
        texturePos.push_back(dis);
        texturePos.push_back(dis);

        positions.push_back(pointss[pointss.size() - 1].x);
        positions.push_back(0);
        positions.push_back(pointss[pointss.size() - 2].y);
        texturePos.push_back(0);
        texturePos.push_back(dis);

        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(i + 2);

        indices.push_back(i + 2);
        indices.push_back(i + 3);
        indices.push_back(i);

        //

        i += 4;


        positions.push_back(pointss[pointss.size() - 1].x);
        positions.push_back(size);
        positions.push_back(pointss[pointss.size() - 1].y);
        texturePos.push_back(0);
        texturePos.push_back(0);



        positions.push_back(pointss[pointss.size() - 2].x);
        positions.push_back(size);
        positions.push_back(pointss[pointss.size() - 1].y);
        texturePos.push_back(dis);
        texturePos.push_back(0);

        positions.push_back(pointss[pointss.size() - 2].x);
        positions.push_back(size);
        positions.push_back(pointss[pointss.size() - 2].y);
        texturePos.push_back(dis);
        texturePos.push_back(dis);

        positions.push_back(pointss[pointss.size() - 1].x);
        positions.push_back(size);
        positions.push_back(pointss[pointss.size() - 2].y);
        texturePos.push_back(0);
        texturePos.push_back(dis);

      indices.push_back(i + 1);
      indices.push_back(i);
      indices.push_back(i + 2);
      
      indices.push_back(i + 3);
      indices.push_back(i + 2);
      indices.push_back(i);
    }

    BufferGroup bg;


    bg.positions = initB(positions);
    bg.texturePos = initB(texturePos);
    bg.indices = initE(indices);
    bg.length = indices.size();
    return  bg;
}
int initB(std::vector<float> pointss) {

    unsigned int B;
    glGenBuffers(1, &B);
    glBindBuffer(GL_ARRAY_BUFFER, B);
    glBufferData(GL_ARRAY_BUFFER, pointss.size() * sizeof(float), pointss.data(), GL_STATIC_DRAW);
    return B;
}
int initE(std::vector<int> pointss) {
    unsigned int B;
    glGenBuffers(1, &B);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, B);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, pointss.size() * sizeof(int), pointss.data(), GL_STATIC_DRAW);
    return B;
}

BufferGroup::BufferGroup() {};
BufferGroup::BufferGroup(GLuint positions, GLuint texturePos, GLuint indices, GLuint length) {
    this->positions = positions;
    this->texturePos = texturePos;
    this->indices = indices;
    this->length = length;
}



float positionsCube[9][12] = {
    {// Front face 0
        -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0,
    } ,{// Back face 1
        -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0,
    },{// Top face 2
        -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0,
    },{// Bottom face 3
        -1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0,
    },{// Right face 4
        1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0,
    },{// Left face 5
        -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0,
    },{// Z face L 8
        -1.0, -1.0, 0.0, 1.0, -1.0, 0.0, 1.0, 1.0, 0.0, -1.0, 1.0, 0.0,
    },{// X face F 6
        0.0, -1.0, -1.0, 0.0, 1.0, -1.0, 0.0, 1.0, 1.0, 0.0, -1.0, 1.0,
    },{// Y face T 7
        -1.0, 0.0, -1.0, -1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 1.0, 0.0, -1.0,

    } };

float textureCoordinatesCube[9][8] = { 
    { // Front
        0.375, 0.75, 0.375, 1, 0.625, 1, 0.625, 0.75,
    }, 
    {// Back
        0.375, 0.5, 0.625, 0.5, 0.625, 0.25, 0.375, 0.25,
    }, 
    {// Top
        0.625, 0.5, 0.875, 0.5, 0.875, 0.25, 0.625, 0.25,
    }, 
    {// Bottom
        0.125, 0.25, 0.125, 0.5, 0.375, 0.5, 0.375, 0.25,
    }, 
    {// Right
        0.375, 0.25, 0.625, 0.25, 0.625, 0, 0.375, 0,
    }, 
    {// Left

        0.375, 0.5, 0.375, 0.75, 0.625, 0.75, 0.625, 0.5,
    }, 
    {// Front da
        1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
    }, 
    {// Front da
        1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
    }, 
    {// Front da
        1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
    } 
};


BufferGroup initCubeBuffer(std::vector<int> i) {

    BufferGroup bg = BufferGroup(initPositionBuffer2(i), initTextureBuffer(i), initIndexBuffer2(i), i.size() * 6);

    return bg;
}

GLuint initIndexBuffer2(std::vector<int> i) {

    std::vector<int> indices;
    for (int j = 0; j < i.size(); j++) {
        std::vector<int> v2 = {
            1 + (4 * j),
            0 + (4 * j),
            2 + (4 * j),


            3 + (4 * j),
            2 + (4 * j),
            0 + (4 * j),

        };
        indices.insert(indices.end(), v2.begin(), v2.end());
    }

    return initE(indices);

}
GLuint initPositionBuffer2(std::vector<int> i) {


    std::vector<float> positions;
    for (int j = 0; j < i.size(); j++) {
        for (int k = 0; k < 12; k++) {
            positions.push_back(positionsCube[i[j]][k]);
        }
    }
    


    return initB(positions);
}
GLuint initTextureBuffer(std::vector<int> i) {
    std::vector<float> textureCoordinates;
    for (int j = 0; j < i.size(); j++) {
        for (int k = 0; k < 8; k++) {
            textureCoordinates.push_back(textureCoordinatesCube[i[j]][k]);
        }
    }


    return initB(textureCoordinates);
}
