#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
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
	// Dpad input controls
	// its values
	bool controlled = false;
	// if the depth is increased
	// it will be controlled
	bool Willcontrol = false;
	// it is allowed to be controlled
	bool selectable = true;
	bool active = true;
	vector<UIelement*> children;

	string text = "";
	int texture = -1;
	virtual ~UIelement() {}
	virtual void draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, bool mouseD);
	virtual void update(vec2 ps, vec2 sc, vec2 mouse, bool mouseD);
	virtual void control(int UpDown, int RightLeft, int InOut, int* level,int depth, float deltatime);
};

class UIDIV:public UIelement
{
public:
	UIDIV(vec2 sca,vec4 back,vec4 four, int hrv);
	int cur = 0;
	int layout = 1;
	virtual void draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, bool mouseD);
	virtual void update(vec2 ps, vec2 sc, vec2 mouse, bool mouseD);
	virtual void control(int UpDown, int RightLeft, int InOut, int* level,int depth, float deltatime);
};
class UIButton :public UIelement
{
public:
	UIButton(vec2 sca, vec4 back, vec4 four, string txt,bool cir);
	bool selected = false;
	bool circle = false;
	UIDIV* child = nullptr;
	virtual void draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, bool mouseD);
	virtual void update(vec2 ps, vec2 sc, vec2 mouse, bool mouseD);
	virtual void control(int UpDown, int RightLeft, int InOut, int* level,int depth, float deltatime);
};
class UIslider :public UIelement
{
public:
	float fraction = 0.5;
	UIslider(vec2 sca, vec4 back, vec4 four, string txt, float frac);
	virtual void draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, bool mouseD);
	virtual void update(vec2 ps, vec2 sc, vec2 mouse, bool mouseD);
	virtual void control(int UpDown, int RightLeft, int InOut, int* level,int depth, float deltatime);
};
class UItoggler :public UIelement
{
public:
	bool big = false;
	bool isTrue = false;
	bool isDown = false;
	UItoggler(vec2 sca, vec4 back, vec4 four, string txt, bool tr,bool bg);
	virtual void draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, bool mouseD);
	virtual void update(vec2 ps, vec2 sc, vec2 mouse, bool mouseD);
	virtual void control(int UpDown, int RightLeft, int InOut, int* level,int depth, float deltatime);
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
	UIDIV* screen;
	UIsettings settings;
	virtual void draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, bool mouseD);
	virtual void update(vec2 ps, vec2 sc, vec2 mouse, bool mouseD);
	virtual void control(int UpDown, int RightLeft, int InOut, int* level,int depth, float deltatime);
};


