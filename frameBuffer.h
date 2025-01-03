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
	GLuint RB;
	GLuint FB;
	glm::vec2 size;
	void bind(bool clear);
	GframeBuffer(glm::vec2 sz);
	GframeBuffer();
	void set(glm::vec2 sz);
};

class FrameBuffer
{
public:
	GLuint ColTex;
	GLuint FB;
	glm::vec2 size;
	void bind(bool clear);
	FrameBuffer(glm::vec2 sz);
	FrameBuffer();
	void set(glm::vec2 sz);
	GLuint RB;
};

class createShadowFramebufferCube {

public:
	GLuint FB;
	GLuint RB;
	std::vector<GLenum> face;
	std::vector<glm::vec3> target;
	std::vector<glm::vec3> up;
	int size;
	void bind(bool clear, int i, GLuint depthTex);
	createShadowFramebufferCube(int sz);
	createShadowFramebufferCube();
};