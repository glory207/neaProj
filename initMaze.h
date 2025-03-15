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
    Landmark(vec2 P, float S);
    Landmark();
    static void set(std::vector<Landmark*>* marks,float min, float max);
};


class CellGrid {
public:
    CellGrid();
    CellGrid(vec2 pos, int x, int y, int p);
    int x;
    int y;
    int p;
    vec2 pos;
    bool visited = false;
    CellGrid* parent = nullptr;
    bool obstruction = false;
    bool isPath = false;
    int Holistic = 0;
    int Distance = INFINITY;
    int DH = INFINITY;
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
    bool connected = false;
    std::vector<Cell*> Conectablednodes;
    std::vector<Cell*> Connectednodes;
    std::vector<Light> ligh;
    std::vector<Furniture> fur;
    std::vector<int> sides;
    bool sidesIsPath[4] = {false,false,false,false};
    glm::vec2 pnt(int i,float thk);
    void conect(Cell* sender);
    void set();

    CellGrid grid[9][9];
    void setSearch(CellGrid* pos);

    void visit(std::vector<Cell*>* nodesToSearch);
};

class Maze
{
public:
    Maze();
    Maze(int c);
    std::vector<glm::vec2> makeLines(int tp,int  sd,int  p0,int  p1,int  p2,int  p3,bool  swch,float thin);
    bool collide(glm::vec3* poss, float leway);
    std::vector<Cell> nodes;
    std::vector<Landmark*> Camps;
    float size;
    int count;
    float thk;
    BufferGroup buffers;
    BufferGroup MapBuffers;
    SpObj obj;
    SpObj obj2;
    float project(float x, float y, int side, Furniture f);
    CellGrid* adjacentCellGrid(int CurentN[3], int px, int py, std::vector<CellGrid*>* nodesToSearch);
    std::vector<CellGrid*> getpath(int sx, int sy, int start, int ex, int ey, int end);
};




class LootSpot : public Landmark{
public:
    LootSpot(vec2 pos);
};



class EnCamp : public Landmark{
public:
    std::vector<Landmark*> cages;
    EnCamp(vec2 pos, float size);
};


bool order(CellGrid* a, CellGrid* b);
