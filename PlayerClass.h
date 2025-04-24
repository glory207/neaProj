#pragma once
#include "object.h"
#include "initBuffer.h"
#include "camera.h"
#include <vector>
#include "animation.h"

class InputObject
{
public:
    vector<int> inventory;
    bool grabed = false;
    SpObj obj;
    int spd;
    float speed;
    float visible = 1;
    float soulStability = 50;
    vec3 pos;
    vec3 rot;
    vec3 vel;
    vec2 inp;
    vec2 lok;
    vec2 whe;
    bool WH;
    bool SP;
    bool SH;
    bool Grounded;
    bool crouched;
    AnimationClass animation;
    vec3 resistance;
    vec2 jump;
    camera* cam;
    vec3 camArm;
    vec3 camArmTarget;
    InputObject() {};
    InputObject(vec3 poss, vec3 rott) {
        spd = 1;
        speed = 0.0;
        pos = poss;
        rot = rott;
        vel = vec3(0);
        inp = vec2(0);
        lok = vec2(0);
        whe = vec2(1,0);
        WH = false;
        SP = false;
        SH = false;
        Grounded = true;
        animation =  AnimationClass(0, 0, 0.0f, 20.0f, 0.0f );

        resistance = vec3(7.0, 7.0, 7.0);
        jump = vec2(0.0, 0.0);
        camArm = vec3(0.27f,0.0f,1.25);
    }
};




class StateClass
{
public:
    InputObject* inp;
    StateClass* cur;
    virtual int Enter() = 0;
    virtual int update(float deltaTime) = 0;
};

class PlayerClass
{

public:
    PlayerClass();
    PlayerClass(vec3 pos, vec3 rot);
    void draw(float time, int shader);
    void update(float deltaTime);
    InputObject* inp;
    StateClass* movable;
    StateClass* cur;
    
};


