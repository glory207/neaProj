#pragma once
#include"object.h"
#include "initMaze.h"
#include <queue>
#include <random>
#include "animation.h"

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
    vec3 pos;
    vec3 rot;
    vec3 vel;
    vec3 lookTarget = vec3(1,0.3,0);
    vec3 look;
    Light vision;
    AnimationClass animation = AnimationClass(35, 36, 10, 0, 2);
};

