#pragma once

#include "glm/glm.hpp"
#include <iostream>
#include "texture.h"
using namespace glm;
using namespace std;

class AnimationClass {
public:
    int colour;
    int normal;
    int framecount;
    float fps;
    float current;
    int dir;
    AnimationClass();
    AnimationClass(int col, int nor,
        int cnt, float cur, float f);
    vec4 sprite();
};

