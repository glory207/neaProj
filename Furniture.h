#pragma once
#include "object.h"
#include "initBuffer.h"
using namespace glm;
class Furniture
{
public:
	Furniture();
	Furniture(int i, int k, int(*grid)[9][9], float size, float thk, vec3 ps);
	SpObj obj;
	int ColorTexture;
	int NormalTexture;
	static bool canFit(int i, int k, int(*grid)[9][9]);
};

