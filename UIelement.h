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
	bool hover;
	bool active = true;
	vector<UIelement*> children;

	int texture = -1;
	virtual ~UIelement() {}
	virtual void draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD);
	virtual void update(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD);
};
class UIDIV:public UIelement
{
public:
	UIDIV(vec2 sca,vec4 back,vec4 four, bool hrv);
	int cur = -1;
	bool HorV = true;
	virtual void draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD);
	virtual void update(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD);
};
class UIButton :public UIelement
{
public:
	UIButton(vec2 sca, vec4 back, vec4 four, string txt);
	UIDIV* child = nullptr;
	string text = " ";
	virtual void draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD);
	virtual void update(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD);
};
class UIslider :public UIelement
{
public:
	float fraction = 0.5;
	string text = " ";
	UIslider(vec2 sca, vec4 back, vec4 four, string txt, float frac);
	virtual void draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD);
	virtual void update(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD);
};
class UItoggler :public UIelement
{
public:
	bool big = false;
	bool isTrue = false;
	bool isDown = false;
	string text = " ";
	UItoggler(vec2 sca, vec4 back, vec4 four, string txt, bool tr,bool bg);
	virtual void draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD);
	virtual void update(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD);
};
void drawString(GLuint VAO, GLuint ShaderUI, vec2 pss, vec2 scc, string text);


class UIsettings {
public:
	// settings
	//		graphics
	float* resolution;
	float* brightness;
	bool* frameLock;
	bool* close;
	//		audio
	float* masterVolume;
	float* musicVolume;
	float* playerVolume;
	float* enemyVolume;
	float* otherVolume;
	//		controls
	float* mouseSensitivity;
	float* controllerSensitivity;
	//		debug
	bool* debug;
	
	// pause
	bool* quitToMain;
	
	// main
	float* gridSize;
	float* enemies;
	bool* gameStart;
	bool* quitGame;
	bool* generate;

	UIsettings();

};

class UImenue :public UIelement {
public:
	UImenue(GLuint texT);
	UIDIV* fullBox;
	UIsettings settings;
	virtual void draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD);
	virtual void update(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD);
};


