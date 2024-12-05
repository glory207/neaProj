#pragma once
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
class GframeBuffer
{
public:
	GLuint ColTex;
	GLuint PosTex;
	GLuint NomTex;
	GLuint NomFTex;
	GLuint FB;
	glm::vec2 size;
	void bind(bool clear);
	GframeBuffer(glm::vec2 sz);
	GframeBuffer();
};

class createShadowFramebufferCube {

public:
	GLuint FB;
	GLuint RB;
	std::vector<GLenum> face;
	std::vector<glm::vec3> target;
	std::vector<glm::vec3> up;
	float size;
	void bind(bool clear, int i, GLuint depthTex);
	createShadowFramebufferCube(float sz);
	createShadowFramebufferCube();
};