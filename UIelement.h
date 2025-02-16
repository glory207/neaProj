#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
using namespace std;
using namespace glm;
class UIelement
{
public:
	vec2 pos;
	vec2 sca;
	vec4 back;
	vec4 four;
	int chr;

	virtual ~UIelement() {}
	virtual void draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD);
};
class UIDIV:public UIelement
{
public:
	UIDIV(vec2 pos, vec2 sca,vec4 back,vec4 four);
	vector<UIelement*> children;
	int cur = -1;
	virtual void draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD);
};

class UIButton :public UIelement
{
public:
	UIButton(vec2 pos, vec2 sca, vec4 back, vec4 four, string txt);
	UIelement* child = nullptr;
	string text = " ";
	virtual void draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD);
};


class UIslider :public UIelement
{
public:
	float fraction = 0.5;
	string text = " ";
	UIslider(vec2 pos, vec2 sca, vec4 back, vec4 four, string txt, float frac);
	virtual void draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD);
};

class UItoggler :public UIelement
{
public:
	bool isTrue = false;
	bool isDown = false;
	string text = " ";
	UItoggler(vec2 pos, vec2 sca, vec4 back, vec4 four, string txt);
	virtual void draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD);
};

void drawString(GLuint VAO, GLuint ShaderUI, vec2 pss, vec2 scc, string text);



