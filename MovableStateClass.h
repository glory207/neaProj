#pragma once
#include "PlayerClass.h"

class MovableState : public StateClass {
public:
    StateClass* idle;
    StateClass* motion;
    StateClass* inAir;
    MovableState() {}
    virtual ~MovableState() {}
    MovableState(InputObject* inp, StateClass* sender);

    virtual int Enter();
    virtual int update(float deltaTime);
    
};


class IdleState : public StateClass {
public:
    StateClass* stood;
    StateClass* crouched;
    MovableState* sender;
    IdleState() {}
    virtual ~IdleState() {}
    IdleState(InputObject* inp, MovableState* sender);

    virtual int Enter();
    virtual int update(float deltaTime);
    
};

class IdleStoodState : public StateClass {
public:
    IdleState* sender;
    IdleStoodState() {}
    virtual ~IdleStoodState() {}
    IdleStoodState(InputObject* inp, IdleState* sender);

    virtual int Enter();
    virtual int update(float deltaTime);
    
};
class IdleCrouchState : public StateClass {
public:
    IdleState* sender;
    IdleCrouchState() {}
    virtual ~IdleCrouchState() {}
    IdleCrouchState(InputObject* inp, IdleState* sender);

    virtual int Enter();
    virtual int update(float deltaTime);
    
};

class MotionState : public StateClass {
public:
    MovableState* sender;
    StateClass* walk;
    StateClass* run;
    StateClass* crawl;
    MotionState() {}
    virtual ~MotionState() {}
    MotionState(InputObject* inp, MovableState* sender);

    virtual int Enter();
    virtual int update(float deltaTime);
    
};


class WalkState : public StateClass {
public:
    MotionState* sender;
    WalkState() {}
    virtual ~WalkState() {}
    WalkState(InputObject* inp, MotionState* sender);

    virtual int Enter();
    virtual int update(float deltaTime);
    
};

class RunState : public StateClass {
public:
    MotionState* sender;
    RunState() {}
    virtual ~RunState() {}
    RunState(InputObject* inp, MotionState* sender);

    virtual int Enter();
    virtual int update(float deltaTime);
    
};

class CrawlState : public StateClass {
public:
    MotionState* sender;
    CrawlState() {}
    virtual ~CrawlState() {}
    CrawlState(InputObject* inp, MotionState* sender);

    virtual int Enter();
    virtual int update(float deltaTime);
    
};










class InAirState : public StateClass {
public:
    MovableState* sender;
    StateClass* dive;
    StateClass* roll;
    StateClass* fall;
    InAirState() {}
    virtual ~InAirState() {}
    InAirState(InputObject* inp, MovableState* sender);

    virtual int Enter();
    virtual int update(float deltaTime);
    
};

class DiveState : public StateClass {
public:
    InAirState* sender;
    DiveState() {}
    virtual ~DiveState() {}
    DiveState(InputObject* inp, InAirState* sender);

    virtual int Enter();
    virtual int update(float deltaTime);
    
};

class RollState : public StateClass {
public:
    InAirState* sender;
    RollState() {}
    virtual ~RollState() {}
    RollState(InputObject* inp, InAirState* sender);

    virtual int Enter();
    virtual int update(float deltaTime);
    
};

class FallingState : public StateClass {
public:
    InAirState* sender;
    FallingState() {}
    virtual ~FallingState() {}
    FallingState(InputObject* inp, InAirState* sender);

    virtual int Enter();
    virtual int update(float deltaTime);
    
};