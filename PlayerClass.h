#pragma once
#include "object.h"
#include "initBuffer.h"
#include "texture.h"
#include <vector>
#include "glm/glm.hpp"
#include <iostream>
using namespace glm;
using namespace std;
class InputObject
{
public:
    int spd;
    float speed;
    vec3 pos;
    vec3 rot;
    vec2 vel;
    vec3 acc;
    vec2 inp;
    vec2 dir;
    bool SP;
    bool SH;
    bool Grounded;
    bool crouched;
    std::vector<float> animation;
    vec3 resistance;
    vec2 jump;
    vec3 cam;
    InputObject() {};
    InputObject(vec3 poss, vec3 rott) {
        spd = 1;
        speed = 0.0;
        pos = poss;
        rot = rott;
        vel = vec2(0.0, 0.0);
        acc = vec3(0.0, 0.0, 0.0);
        inp = vec2(0, 0);
        dir = vec2(0, 0);
        SP = false;
        SH = false;
        Grounded = true;
        // colour normal framecount current frame;
        animation = { 0.0f, 0.0f, 0.0f, 20.0f, 0.0f };

        resistance = vec3(7.0, 7.0, 7.0);
        jump = vec2(0.0, 0.0);
        cam = vec3(0.0, 0.0, 0.0);
    }
};




class StateClass
{
public:

    InputObject* inp;
    StateClass* cur;
    virtual int Enter() = 0;
    virtual int update(float deltaTime) = 0;
    virtual void Exit() = 0;

};

class PlayerClass
{

public:
    PlayerClass();
    PlayerClass(vec3 pos, vec3 rot);
    void draw(float time, int shader);
    void update(float deltaTime);
    InputObject* inp;
    SpObj obj;
    StateClass* movable;
    StateClass* cur;
    
};

