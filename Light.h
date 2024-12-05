#pragma once
#include "initBuffer.h"
#include "object.h"


class Light
{
public:
	Light();
	GLuint depthTex;
	Light(glm::vec3 pos);
	void update(float deltaTime);
	glm::vec3 acc;
	glm::vec3 pos;
	glm::vec3 col;
	glm::vec3 rot;
	SpObj obj;
};

