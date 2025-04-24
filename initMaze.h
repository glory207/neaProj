# pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <stdlib.h>
#include "initBuffer.h"
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
    // its index in the first dimension
    int x;
    // its index in the second dimension
    int y;
    // the index of the cell its in
    int p;
    // its position in the world
    vec2 pos;
    // is there an object in it
    int obstruction = 0;
    float hight = 0;

    // is there an object in it
    bool visited = false;
    CellGrid* parent = nullptr;
    bool isPath = false;
    int Holistic = 0;
    int Distance = INFINITY;
};


class Furniture
{
public:
    Furniture();
    Furniture(int i, int k, CellGrid(*grid)[9][9], float size, float thk, vec3 ps);
    SpObj obj;
    int ColorTexture;
    int NormalTexture;
    static bool canFit(int i, int k, CellGrid(*grid)[9][9]);
};


class Cell
{
public:
    Cell(int index, int count, float thk);
    int index;
    int count;
    float prob;
    float completion = 0.2f;
    int prize = -1;
    int tresure;
    int x;
    int y;
    bool cage = false;
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
    void distroy();
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
    void adjacentCellGrid(CellGrid* CurentN, int px, int py, std::vector<CellGrid*>* nodesToSearch);
    std::vector<CellGrid*> getpath(CellGrid* start, CellGrid* end);
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
