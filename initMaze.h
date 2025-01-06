# pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <stdlib.h>
#include "initBuffer.h"
#include "Furniture.h"
#include "Light.h"
#include "object.h"
#include <iostream>


class Landmark {
public:
    vec2 Pos;
    float Size;

    virtual ~Landmark() {}
};
class Cell
{
public:
    Cell(int index, int count, float thk);
    int index;
    int count;
    float prob;
    int tresure;
    int x;
    int y;
    bool treasure = false;
    bool conected = false;
    std::vector<Cell*> Conectablednodes;
    std::vector<Cell*> Conectednodes;
    std::vector<int> sides;
    bool sidesi[4] = {false,false,false,false};
    glm::vec2 pnt(int i,float thk);
    void conect(Cell* sender);
    void set();
    
};


class Maze
{
public:
    Maze(std::vector<Light>* ligh, int c);
    std::vector<glm::vec2> makeLines(int tp,int  sd,int  p0,int  p1,int  p2,int  p3,bool  swch,float thin);
    bool collide(glm::vec3* poss, glm::vec3* accc, glm::vec2 leway);
    void drawMap(int programInfo);
    std::vector<Cell> nodes;
    std::vector<Furniture> fur;
    std::vector<Landmark*> Camps;

    InsObj furn;
    float size;
    int count;
    float thk;
    BufferGroup buffers;
    BufferGroup MapBuffers;
    SpObj obj;
    SpObj obj2;
    void draw(int programInfo);
    float project(float x, float y, int side, Furniture f);

    GLuint VAO;
};




class LootSpot : public Landmark{
public:
    LootSpot(vec2 pos);
};



class EnCamp : public Landmark{
public:
    std::vector<vec2> cages;
    EnCamp(vec2 pos, float size);
};



