#pragma once
#include "object.h"
#include "initBuffer.h"
using namespace glm;
class Furniture
{
public:
	Furniture();
	Furniture(vec3 pos, vec3 sca, float rt);
	vec3 pos;
	vec3 sca;
	vec3 rot;
	int type;
};

