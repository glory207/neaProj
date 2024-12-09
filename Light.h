#pragma once
#include "initBuffer.h"
#include "object.h"


class Light
{
public:
	Light();
	GLuint depthTex;
	GLuint64 handle;
	glm::mat4 rotationMatrix;
	Light(glm::vec3 pos);
	void update(float deltaTime);
	glm::vec3 acc;
	glm::vec3 pos;
	glm::vec3 col;
	glm::vec3 rot;
	SpObj obj;
};

