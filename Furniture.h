#pragma once
#include "object.h"
#include "initBuffer.h"
using namespace glm;
class Furniture
{
public:
	Furniture();
	Furniture(int i, int k, int(*grid)[3][3], float size, float thk, vec3 ps);
	vec3 pos;
	vec3 sca;
	vec3 rot;
	int type1;
	int type2;
	static bool canFit(int i, int k, int(*grid)[3][3]);
};

