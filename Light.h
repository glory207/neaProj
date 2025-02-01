#pragma once
#include "initBuffer.h"
#include "object.h"
#include "frameBuffer.h"


class Light
{
public:
	Light();
	bool active = false;
	bool active2 = true;
	void activate(bool act);
	GLuint depthTex;
	GLuint depthTexPre;
	GLuint64 handle;
	glm::mat4 rotationMatrix;
	int size;
	Light(glm::vec3 pos);
	void update(float deltaTime);
	glm::vec3 acc;
	glm::vec3 pos;
	glm::vec3 col;
	glm::vec3 rot;
	SpObj obj;
};

