#pragma once
#include "initMaze.h"
#include <queue>
#include <random>
#include "PlayerClass.h"


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

class EnemyInputObject {
public:
    float focus = 0;
    SpObj obj;
    PathFind pathFinder;
    vec3 pos;
    vec3 rot;
    vec3 vel;
    vec3 lookTarget = vec3(1, 0.3, 0);
    vec3 look;
    PlayerClass* player;
    Light vision;
    AnimationClass animation = AnimationClass(35, 36, 10, 0, 2);
    EnemyInputObject();
};



class EnemyStateClass
{
public:
    EnemyInputObject* inp;
    EnemyStateClass* cur;
    virtual int Enter() = 0;
    virtual int update(float deltaTime, Maze* mz, queue<PathFind*>* pathfq) = 0;
};

class EnemyState : public EnemyStateClass {
public:

    EnemyStateClass* patrol;
    EnemyStateClass* chase;
    EnemyState() {}
    virtual ~EnemyState() {}
    EnemyState(EnemyInputObject* inp);

    virtual int Enter();
    virtual int update(float deltaTime, Maze* mz, queue<PathFind*>* pathfq);
};

class Enemy
{
public:

    Enemy();
    Enemy(PlayerClass* pla,vec3 pos, vec3 rot);
    void draw(float time, int shader);
    void update(float deltaTime, vec3 cam, Maze* mz, queue<PathFind*>* pathfq);
    EnemyInputObject* inp;
    EnemyState* cur;
};

class PatrollingStae : public EnemyStateClass {
public:
    EnemyStateClass* idle;
    EnemyStateClass* walk;
    EnemyState* sender;
    PatrollingStae() {}
    virtual ~PatrollingStae() {}
    PatrollingStae(EnemyInputObject* inp, EnemyState* sender);

    virtual int Enter();
    virtual int update(float deltaTime, Maze* mz, queue<PathFind*>* pathfq);
};
class EidleState : public EnemyStateClass {
public:
    PatrollingStae* sender;
    EidleState() {}
    virtual ~EidleState() {}
    EidleState(EnemyInputObject* inp, PatrollingStae* sender);
    float timer;
    virtual int Enter();
    virtual int update(float deltaTime, Maze* mz, queue<PathFind*>* pathfq);
};
class EwalkingState : public EnemyStateClass {
public:
    PatrollingStae* sender;
    EwalkingState() {}
    virtual ~EwalkingState() {}
    EwalkingState(EnemyInputObject* inp, PatrollingStae* sender);

    virtual int Enter();
    virtual int update(float deltaTime, Maze* mz, queue<PathFind*>* pathfq);
};
class ChasingState : public EnemyStateClass {
public:
    EnemyState* sender;
    EnemyStateClass* follow;
    EnemyStateClass* drain;
    ChasingState() {}
    virtual ~ChasingState() {}
    ChasingState(EnemyInputObject* inp, EnemyState* sender);

    virtual int Enter();
    virtual int update(float deltaTime, Maze* mz, queue<PathFind*>* pathfq);
};

class followingState : public EnemyStateClass {
public:
    float timer;
    ChasingState* sender;
    followingState() {}
    virtual ~followingState() {}
    followingState(EnemyInputObject* inp, ChasingState* sender);

    virtual int Enter();
    virtual int update(float deltaTime, Maze* mz, queue<PathFind*>* pathfq);
};
class drainingState : public EnemyStateClass {
public:

    float timer;
    ChasingState* sender;
    drainingState() {}
    virtual ~drainingState() {}
    drainingState(EnemyInputObject* inp, ChasingState* sender);

    virtual int Enter();
    virtual int update(float deltaTime, Maze* mz, queue<PathFind*>* pathfq);
};


