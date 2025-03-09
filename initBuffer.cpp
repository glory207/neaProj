#include "initBuffer.h"
#include <iostream>


BufferGroup initMaze2DBuffers(std::vector<glm::vec2> points) {

    std::vector<float> positions;
    std::vector<int> indices;
    // sepperates the point out into xyz components and stores them into a buffer
    for (int i = 0; i < points.size(); i += 1) {
        
        positions.push_back(points[i].y); 
        positions.push_back(points[i].x); 
        positions.push_back(0.0f); 
        // The maze is drawn with lines not triangles so the order can be sequencial 
        indices.push_back((i));
    }

    BufferGroup bg = BufferGroup(initB(positions), initE(indices), indices.size());

    return  bg;
}


BufferGroup initMazeBuffers(std::vector<glm::vec2> points) {
    std::vector<float> positions;
    std::vector<float> texturePos;
    std::vector<int> indices;
    // the scale of a rooms height
    float size = 0.5f;
    for (int i = 0; i < points.size() - 2; i += 2) {

        // using distance dis the scale of textures wont be warped by the vairing wall sizes
        float dis = glm::sqrt(glm::pow(points[i].x - points[i + 1].x, 2.0f) + glm::pow(points[i].y - points[i + 1].y, 2.0f));

        // takes two points then extrudes them upwards to form a wall

        positions.push_back(points[i + 1].x);
        positions.push_back(0);
        positions.push_back(points[i + 1].y);
        texturePos.push_back(dis);
        texturePos.push_back(size);

        positions.push_back(points[i].x);
        positions.push_back(0);
        positions.push_back(points[i].y);
        texturePos.push_back(0);
        texturePos.push_back(size);


        positions.push_back(points[i].x);
        positions.push_back(size);
        positions.push_back(points[i].y);
        texturePos.push_back(0);
        texturePos.push_back(0);

        positions.push_back(points[i + 1].x);
        positions.push_back(size);
        positions.push_back(points[i + 1].y);
        texturePos.push_back(dis);
        texturePos.push_back(0);


        // splits the 4 vertexes of the wall into two triangles

        indices.push_back((i * 2) + 1);
        indices.push_back((i * 2));
        indices.push_back((i * 2) + 2);
       
        indices.push_back((i * 2) + 3);
        indices.push_back((i * 2) + 2);
        indices.push_back((i * 2) );

    }


    {

        int i = ((points.size() - 4) * 2) + 4;
        // using distance dis the scale of textures wont be warped by the vairing floor and ceiling sizes
        float dis = (points[points.size() - 1].x - points[points.size() - 2].x);

        // using the 2 corners of the map the floor is created
        positions.push_back(points[points.size() - 1].x);
        positions.push_back(0);
        positions.push_back(points[points.size() - 1].y);
        texturePos.push_back(0);
        texturePos.push_back(0);



        positions.push_back(points[points.size() - 2].x);
        positions.push_back(0);
        positions.push_back(points[points.size() - 1].y);
        texturePos.push_back(dis);
        texturePos.push_back(0);

        positions.push_back(points[points.size() - 2].x);
        positions.push_back(0);
        positions.push_back(points[points.size() - 2].y);
        texturePos.push_back(dis);
        texturePos.push_back(dis);

        positions.push_back(points[points.size() - 1].x);
        positions.push_back(0);
        positions.push_back(points[points.size() - 2].y);
        texturePos.push_back(0);
        texturePos.push_back(dis);
        // splits the 4 vertexes of the floor into two triangles
        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(i + 2);

        indices.push_back(i + 2);
        indices.push_back(i + 3);
        indices.push_back(i);

        //

        i += 4;

        return BufferGroup(initB(positions), initB(texturePos), initE(indices), indices.size());
        // using the 2 corners of the map the ceiling is created

        positions.push_back(points[points.size() - 1].x);
        positions.push_back(size);
        positions.push_back(points[points.size() - 1].y);
        texturePos.push_back(0);
        texturePos.push_back(0);



        positions.push_back(points[points.size() - 2].x);
        positions.push_back(size);
        positions.push_back(points[points.size() - 1].y);
        texturePos.push_back(dis);
        texturePos.push_back(0);

        positions.push_back(points[points.size() - 2].x);
        positions.push_back(size);
        positions.push_back(points[points.size() - 2].y);
        texturePos.push_back(dis);
        texturePos.push_back(dis);

        positions.push_back(points[points.size() - 1].x);
        positions.push_back(size);
        positions.push_back(points[points.size() - 2].y);
        texturePos.push_back(0);
        texturePos.push_back(dis);
        // splits the 4 vertexes of the ceiling into two triangles
      indices.push_back(i + 1);
      indices.push_back(i);
      indices.push_back(i + 2);
      
      indices.push_back(i + 3);
      indices.push_back(i + 2);
      indices.push_back(i);
    }

    return BufferGroup(initB(positions), initB(texturePos), initE(indices), indices.size());

}


int initB(std::vector<float> points) {
    // creates space in memory for a buffer fills it then returns a pointer to it 
    unsigned int B;
    glGenBuffers(1, &B);
    glBindBuffer(GL_ARRAY_BUFFER, B);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_STATIC_DRAW);
    return B;
}

int initE(std::vector<int> points) {
    // creates space in memory for a buffer fills it then returns a pointer to it 
    unsigned int B;
    glGenBuffers(1, &B);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, B);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, points.size() * sizeof(int), points.data(), GL_STATIC_DRAW);
    return B;
}

BufferGroup::BufferGroup() {};
BufferGroup::BufferGroup(GLuint positions, GLuint indices, GLuint length) {

    // vertex atrribute object combines the buffers to be sent to the GPU
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, positions);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    this->length = length;
}
BufferGroup::BufferGroup(GLuint positions, GLuint texturePos, GLuint indices, GLuint length) {

    // vertex atrribute object combines the buffers to be sent to the GPU
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, positions);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, texturePos);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    this->length = length;
}


// the vertex data of a cube

float positionsCube[12][12] = {
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
    },{// Z face L 6
        -1.0, -1.0, 0.0, 1.0, -1.0, 0.0, 1.0, 1.0, 0.0, -1.0, 1.0, 0.0,
    },{// X face F 7
        0.0, -1.0, -1.0, 0.0, 1.0, -1.0, 0.0, 1.0, 1.0, 0.0, -1.0, 1.0,
    },{// Y face T 8
        -1.0, 0.0, -1.0, -1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 1.0, 0.0, -1.0,

    },{// X face full 9
        -1.0, -1.0, 0.0, 1.0, -1.0, 0.0, 1.0, 1.0, 0.0, -1.0, 1.0, 0.0,
    },{// Y face full 10
        0.0, -1.0, -1.0, 0.0, 1.0, -1.0, 0.0, 1.0, 1.0, 0.0, -1.0, 1.0,
    },{// Z face full 11
        -1.0, 0.0, -1.0, -1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 1.0, 0.0, -1.0,

    } };

float textureCoordinatesCube[12][8] = { 
    { // Front
        0.375, 0.75, 0.375, 1, 0.625, 1, 0.625, 0.75,
    },{// Back
        0.375, 0.5, 0.625, 0.5, 0.625, 0.25, 0.375, 0.25,
    },{// Top
        0.625, 0.5, 0.875, 0.5, 0.875, 0.25, 0.625, 0.25,
    },{// Bottom
        0.125, 0.25, 0.125, 0.5, 0.375, 0.5, 0.375, 0.25,
    },{// Right
        0.375, 0.25, 0.625, 0.25, 0.625, 0, 0.375, 0,
    },{// Left

        0.375, 0.5, 0.375, 0.75, 0.625, 0.75, 0.625, 0.5,
    }, {// X face
        0.0, 0.75, 0.0, 0.5, 0.25, 0.5,0.25, 0.75,
    }, {// Y face
        0.75, 0.75, 1.0, 0.75, 1.0, 1.0, 0.75, 1.0,
    },{// Z face
         0.25, 1.0, 0.25, 0.75,  0.0, 0.75,0.0, 1.0,
    },{// X face full
        1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
    },{// Y face full
        1.0, 1.0, 1.0, 0.0,0.0, 0.0,0.0, 1.0,
    },{// Z face full
         1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
    }
};


BufferGroup initCubeBuffer(std::vector<int> sides) {
    // creates a buffer group
    return BufferGroup(initPositionBuffer(sides), initTextureBuffer(sides), initIndexBuffer(sides), sides.size() * 6);
}

GLuint initIndexBuffer(std::vector<int> sides) {

    std::vector<int> indices;
    for (int j = 0; j < sides.size(); j++) {
        // the 4 vertexes of a side 0-3 must be split into two triangles 
        std::vector<int> v2 = {
            1 + (4 * j),
            0 + (4 * j),
            2 + (4 * j),


            3 + (4 * j),
            2 + (4 * j),
            0 + (4 * j),

        };
        // They get added to the list of triangles
        indices.insert(indices.end(), v2.begin(), v2.end());
    }

    return initE(indices);
}

GLuint initPositionBuffer(std::vector<int> sides) {

    // adds the selected sides
    std::vector<float> positions;
    for (int j = 0; j < sides.size(); j++) {
        // the x y and z positions of the 4 corners of a face
        for (int k = 0; k < 12; k++) {
            positions.push_back(positionsCube[ sides[j] ] [ k]);
        }
    }

    return initB(positions);
}
GLuint initTextureBuffer(std::vector<int> sides) {

    // adds the selected sides
    std::vector<float> textureCoordinates;
    for (int j = 0; j < sides.size(); j++) {
            // the x and y texture positions of the 4 corners of a face
        for (int k = 0; k < 8; k++) {
            textureCoordinates.push_back(textureCoordinatesCube[ sides[j] ][k]);
        }
    }

    return initB(textureCoordinates);
}
