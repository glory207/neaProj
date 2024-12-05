# pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <stdlib.h>
#include "initBuffer.h"
#include "object.h"
#include <iostream>


class Cell
{
public:
    Cell(int index, int count, float thk);
    int index;
    int count;
    int x;
    int y;
    bool conected = false;
    std::vector<Cell*> Conectablednodes;
    std::vector<Cell*> Conectednodes;
    std::vector<int> sides;
    bool sidesi[4] = {false,false,false,false};
    std::vector<glm::vec2> pnt;
  public:  void conect(Cell* sender);
    void set();
};

class Maze
{
public:
    Maze();
    std::vector<glm::vec2> makeLines(int tp,int  sd,int  p0,int  p1,int  p2,int  p3,bool  swch,float thin);
    bool collide(glm::vec3* poss, glm::vec3* accc, glm::vec2 leway);
    void drawMap(int programInfo);
    std::vector<Cell> nodes;
    float size;
    int count;
    float thk;
    BufferGroup buffers;
    SpObj obj;
};


