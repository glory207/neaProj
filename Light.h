#pragma once
#include "initBuffer.h"
#include "object.h"
#include "frameBuffer.h"

class Light
{
public:
	Light();
	bool active = false;
	bool firstActive = true;
	void activate(bool act);
	GLuint depthTex;
	GLuint depthTexPre;
	GLuint64 handle;
	glm::mat4 rotationMatrix;
	int perch[2] = {-1,-1};
	int size;
	Light(glm::vec3 pos);
	glm::vec3 acc;
	glm::vec3 pos;
	glm::vec3 col;
	glm::vec3 rot;
	SpObj obj;
};

