#pragma once
#include "initBuffer.h"
#include "object.h"


class Light
{
public:
	Light();
	bool active = false;
	bool active2 = true;
	void activate(bool act);
	GLuint depthTex;
	GLuint64 handle;
	glm::mat4 rotationMatrix;
	float size;
	Light(glm::vec3 pos, int sz);
	void update(float deltaTime);
	glm::vec3 acc;
	glm::vec3 pos;
	glm::vec3 col;
	glm::vec3 rot;
	SpObj obj;
};

