#pragma once
#include"object.h"
#include "initMaze.h"
#include <queue>
#include <random>
using namespace glm;
using namespace std;
class PathFind
{
public:
    PathFind();
    void FindPath(vec3 startpos, vec3 endpos, queue<PathFind*>* pathfq);
    vec3 Startpos;
    vec3 Endpos;
    float pathP = 0.0;
    bool OnPath = false;
    bool EndPath = false;
    bool WaitingForPath = false;
    std::vector<CellGrid*> path;

};
class Enemy
{
public:

    Enemy();
    Enemy(vec3 pos, vec3 rot);
    void draw(float time, int shader);
    void update(float deltaTime, vec3 cam, Maze* mz, queue<PathFind*>* pathfq);
    SpObj obj;
    PathFind pathFinder;
    float frame = 0.0;
    int dir = 0;
    vec3 pos;
    vec3 rot;
    vec3 acc = vec3(0,0,0);
    Light vision;
};

