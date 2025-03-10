#include "UIelement.h"

void UIelement::draw(GLuint VAO,GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD) {

	glUniform1i(glGetUniformLocation(ShaderUI, "chr"), chr);
	glUniform1f(glGetUniformLocation(ShaderUI, "corner"), 0.05);
	glUniform4f(glGetUniformLocation(ShaderUI, "backround"), back.x, back.y, back.z, back.w);
	glUniform4f(glGetUniformLocation(ShaderUI, "textureMatrix"), pos.x * sc.x + ps.x, pos.y * sc.y + ps.y, sca.x * sc.x, sca.y * sc.y);
	glUniform4f(glGetUniformLocation(ShaderUI, "forground"), four.x, four.y, four.z, four.w);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

UIDIV::UIDIV(vec2 pos, vec2 sca, vec4 back, vec4 four) {
	this->pos = pos;
	this->sca = sca;
	this->back = back;
	this->four = four;
	chr = -1;
}
void UIDIV::draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD) {
	UIelement::draw(VAO, ShaderUI, ps, sc, mouse, mouseD);

	for (int n = 0; n < children.size(); n ++) {
		if (dynamic_cast<UIDIV*>(children[n]) != nullptr)(*children[n]).draw(VAO, ShaderUI, pos * sc + ps, sca * sc, mouse, mouseD);
		if (dynamic_cast<UIButton*>(children[n]) != nullptr)
		{
			UIButton* buton = dynamic_cast<UIButton*>(children[n]);
			if (mouseD&&
				mouse.x > (pos.x * sc.x + ps.x + ((*buton).pos.x * sca.x * sc.x)) - (sca.x * sc.x * (*buton).sca.x) &&
				mouse.x < (pos.x * sc.x + ps.x + ((*buton).pos.x * sca.x * sc.x)) + (sca.x * sc.x * (*buton).sca.x) &&
				mouse.y >(pos.y * sc.y + ps.y + ((*buton).pos.y * sca.y * sc.y)) - (sca.y * sc.y * (*buton).sca.y) &&
				mouse.y < (pos.y * sc.y + ps.y + ((*buton).pos.y * sca.y * sc.y)) + (sca.y * sc.y * (*buton).sca.y) 
				
				)
				cur = n;
			if(cur == n && (*buton).child != nullptr)(*buton).child->draw(VAO, ShaderUI, ps, sc, mouse, mouseD);
		}
		(*children[n]).draw(VAO, ShaderUI, pos * sc + ps, sca * sc, mouse, mouseD);
	}
}
UIButton::UIButton(vec2 pos, vec2 sca, vec4 back, vec4 four,string txt) {
	this->pos = pos;
	this->sca = sca;
	this->back = back;
	this->four = four;
	this->text = txt;
	chr = -1;
}
void UIButton::draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD) {
	chr = -1;

	bool hover =
		mouse.x < ((pos.x * sc.x + ps.x) + (sca.x * sc.x)) &&
		mouse.x >((pos.x * sc.x + ps.x) - (sca.x * sc.x)) &&
		mouse.y < ((pos.y * sc.y + ps.y) + (sca.y * sc.y)) &&
		mouse.y >((pos.y * sc.y + ps.y) - (sca.y * sc.y))
		;
	vec2 pss;
	vec2 scc = sca * sc;
	if (hover && mouseD) {
		pss = pos * sc + ps + vec2(0.005);
		UIelement::draw(VAO, ShaderUI, ps + vec2(0.005), sc, mouse, mouseD);
	} else if (hover) {
		pss = pos * sc + ps + vec2(0.01);
		UIelement::draw(VAO, ShaderUI, ps + vec2(0.01), sc, mouse, mouseD);
	}
	else
	{
		pss = pos * sc + ps;
		UIelement::draw(VAO, ShaderUI, ps, sc, mouse, mouseD);
	}


	if (mouseD && hover)glUniform4f(glGetUniformLocation(ShaderUI, "forground"), 0.5, 0.5, 0, 1);
	else if (hover)glUniform4f(glGetUniformLocation(ShaderUI, "forground"), 0, 0.5, 0, 1);
	else glUniform4f(glGetUniformLocation(ShaderUI, "forground"), 1, 1, 1, 1);

	drawString(VAO, ShaderUI, pss, scc, text);
	
}


UIslider::UIslider(vec2 pos, vec2 sca, vec4 back, vec4 four, string txt, float frac) {
	this->pos = pos;
	this->sca = sca;
	this->back = back;
	this->four = four;
	this->text = txt;
	fraction = frac;
	chr = -1;
}

void UIslider::draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD) {
	chr = -1;
	bool hover =
		mouse.x < ((pos.x * sc.x + ps.x) + (sca.x * sc.x)) &&
		mouse.x >((pos.x * sc.x + ps.x) - (sca.x * sc.x)) &&
		mouse.y < ((pos.y * sc.y + ps.y) + (sca.y * sc.y)) &&
		mouse.y >((pos.y * sc.y + ps.y) - (sca.y * sc.y));

	UIelement::draw(VAO, ShaderUI, ps, sc, mouse, mouseD);
	if (hover && mouseD) {
		fraction = (mouse.x-((pos.x * sc.x + ps.x) - (sca.x * sc.x)*0.9)) / ((sca.x * sc.x) * 2.0*0.9);
		if (fraction < 0)fraction = 0;
		if (fraction > 1)fraction = 1;
	}

	glUniform1f(glGetUniformLocation(ShaderUI, "corner"), 0.02);
	glUniform4f(glGetUniformLocation(ShaderUI, "backround"), 1, 1, 0, 1);
	glUniform4f(glGetUniformLocation(ShaderUI, "forground"), 1, 1, 0, 1);
	glUniform1i(glGetUniformLocation(ShaderUI, "chr"), -1);
	glUniform4f(glGetUniformLocation(ShaderUI, "textureMatrix"), pos.x* sc.x + ps.x + (sca.x * sc.x*0.9 * (fraction-1.0)),-0.4 * sca.y * sc.y + pos.y* sc.y + ps.y, sca.x * sc.x* (fraction)*0.9, sca.y* sc.y * 0.4);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	glUniform4f(glGetUniformLocation(ShaderUI, "forground"), 0.2, 0.2, 0.2, 1);

	string t = text + " " + to_string(int(fraction * 100)) + "%";
	drawString(VAO, ShaderUI, vec2(0, 0.75) * vec2(1, 0.5) * (sca * sc) + (pos * sc + ps),vec2(1,0.5) * (sca * sc), t);
}

UItoggler::UItoggler(vec2 pos, vec2 sca, vec4 back, vec4 four, string txt, bool tr) {
	this->pos = pos;
	this->sca = sca;
	this->back = back;
	this->four = four;
	this->text = txt;
	this->isTrue = tr;
	chr = -1;
}

void UItoggler::draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD) {
	chr = -1;
	vec2 poss = pos * sc + ps;
	vec2 scaa = sca * sc;

	vec2 oss = vec2(0.75,0.0);
	vec2 caa = vec2(0.15, 0.8);

	bool hover =
		mouse.x < ((oss.x * scaa.x + poss.x) + (caa.x * scaa.x)) &&
		mouse.x >((oss.x * scaa.x + poss.x) - (caa.x * scaa.x)) &&
		mouse.y < ((oss.y * scaa.y + poss.y) + (caa.y * scaa.y)) &&
		mouse.y >((oss.y * scaa.y + poss.y) - (caa.y * scaa.y));
	

	if (hover && mouseD) {
		poss += vec2(0,0.005);
	}
	else if (hover) {
		poss += vec2(0,0.01);
	}

	UIelement::draw(VAO, ShaderUI, ps, sc, mouse, mouseD);

	if (hover && mouseD && isDown) {
		isTrue = !isTrue;
	}
	if (hover && mouseD) isDown = true; 
	else isDown = false;



	glUniform1i(glGetUniformLocation(ShaderUI, "chr"), chr); 
	glUniform1f(glGetUniformLocation(ShaderUI, "corner"), 0.05); 

	
	glUniform4f(glGetUniformLocation(ShaderUI, "textureMatrix"), oss.x * scaa.x + poss.x, oss.y * scaa.y + poss.y, caa.x * scaa.x, caa.y * scaa.y);
	glUniform4f(glGetUniformLocation(ShaderUI, "backround"), 1, 1, 1, 1);
	glBindVertexArray(VAO); 
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
	caa *= 0.6;
	glUniform1f(glGetUniformLocation(ShaderUI, "corner"), 0.025);
	glUniform4f(glGetUniformLocation(ShaderUI, "textureMatrix"), oss.x * scaa.x + poss.x, oss.y * scaa.y + poss.y, caa.x * scaa.x, caa.y * scaa.y);
	if (isTrue) glUniform4f(glGetUniformLocation(ShaderUI, "backround"), 0.2, 0.2, 0.2, 1);

	glUniform4f(glGetUniformLocation(ShaderUI, "forground"), 0,0,0,1);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	drawString(VAO, ShaderUI, vec2(-0.3, 0.0) * vec2(0.7, 0.5) * (sca * sc) + (pos * sc + ps),vec2(0.7,0.5) * (sca * sc), text);
}

void drawString(GLuint VAO, GLuint ShaderUI, vec2 pss, vec2 scc,string text) {
	vec2 po = vec2(0);
	int sz = text.size() + 3;
	vec2 sa = vec2(0.9 / sz, 0.5);
	for (int i = 0; i < text.size(); i++)
	{

		glUniform4f(glGetUniformLocation(ShaderUI, "backround"), 0, 0, 0, 0);
		if (tolower(int(text[i])) >= 97 && tolower(int(text[i])) <= 122)glUniform1i(glGetUniformLocation(ShaderUI, "chr"), tolower(int(text[i])) - 87);
		else if (tolower(int(text[i])) >= 48 && int(text[i]) <= 57)glUniform1i(glGetUniformLocation(ShaderUI, "chr"), tolower(int(text[i])) - 48);
		else if (tolower(int(text[i])) == 45)glUniform1i(glGetUniformLocation(ShaderUI, "chr"), -7);
		else if (tolower(int(text[i])) == 61)glUniform1i(glGetUniformLocation(ShaderUI, "chr"), -6);
		else if (tolower(int(text[i])) == 46)glUniform1i(glGetUniformLocation(ShaderUI, "chr"), -5);
		else if (tolower(int(text[i])) == 37)glUniform1i(glGetUniformLocation(ShaderUI, "chr"), -4);
		else if (tolower(int(text[i])) == 44)glUniform1i(glGetUniformLocation(ShaderUI, "chr"), -3);
		else if (tolower(int(text[i])) == 33)glUniform1i(glGetUniformLocation(ShaderUI, "chr"), -2);
		else glUniform1i(glGetUniformLocation(ShaderUI, "chr"), -1);
		po.x = (2.0 + i * 2) / float(sz) - 1;
		glUniform4f(glGetUniformLocation(ShaderUI, "textureMatrix"), po.x * scc.x + pss.x, po.y * scc.y + pss.y, sa.x * scc.x, sa.y * scc.y);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}

UImenue::UImenue() {
	
	pos = vec2(0);
	sca = vec2(0.95);
	back = vec4(vec3(0.6), 1.0f);
	four = vec4(1.0f);
	chr = -1;

	UIDIV* topBar = new UIDIV(vec2(0, 0.85), vec2(0.85f, 0.1), vec4(vec3(0.2), 1.0f), vec4(1.0f));
	this->children.push_back(topBar);
	UIButton* menueButton = new UIButton(vec2(-0.67, 0), vec2(0.2f, 0.7), vec4(vec3(0.6), 1.0f), vec4(1.0f), "menue");
	UIButton* settingsButton = new UIButton(vec2(-0.2, 0), vec2(0.2f, 0.7), vec4(vec3(0.6), 1.0f), vec4(1.0f), "settings");
	topBar->children.push_back(menueButton);
	topBar->children.push_back(settingsButton);
	UIDIV* settingsBox = new UIDIV(vec2(0, -0.1), vec2(0.95f, 0.8), vec4(vec3(0.2), 1.0f), vec4(1.0f));
	UIDIV* menueBox = new UIDIV(vec2(0, -0.1), vec2(0.95f, 0.8), vec4(vec3(0.2), 1.0f), vec4(1.0f));
	settingsButton->child = settingsBox;
	menueButton->child = menueBox;
	UIslider* gridSizeSlider = new UIslider(vec2(0, 0.75), vec2(0.9, 0.2), vec4(vec3(0.9), 1.0f), vec4(1.0f), "Grid size (only set once)", 0.3);
	UIslider* enemieSlider = new UIslider(vec2(0, 0.3), vec2(0.9, 0.2), vec4(vec3(0.9), 1.0f), vec4(1.0f), "Enemie count size (only set once)", 0.3);
	UItoggler* startGameToggl = new UItoggler(vec2(0, -0.05), vec2(0.9, 0.1), vec4(vec3(0.9), 1.0f), vec4(1.0f), "start game     ", false);
	UIDIV* settingsBar = new UIDIV(vec2(-0.6, 0), vec2(0.35f, 0.9f), vec4(vec3(0.6), 1.0f), vec4(1.0f));
	UIButton* graphicsButton = new UIButton(vec2(0, 0.85), vec2(0.9f, 0.09f), vec4(vec3(0.85), 1.0f), vec4(1.0f), "graphics");
	UIButton* dificultyButton = new UIButton(vec2(0, 0.65), vec2(0.9f, 0.09f), vec4(vec3(0.85), 1.0f), vec4(1.0f), "dificulty");
	UIDIV* graphicsBox = new UIDIV(vec2(0.375, 0), vec2(0.575f, 0.9), vec4(vec3(0.6), 1.0f), vec4(1.0f));
	UIDIV* dificultyBox = new UIDIV(vec2(0.375, 0), vec2(0.575f, 0.9), vec4(vec3(0.6), 1.0f), vec4(1.0f));
	UIslider* brightnessSlider = new UIslider(vec2(0, 0.75), vec2(0.9, 0.2), vec4(vec3(0.9), 1.0f), vec4(1.0f), "brightness", 0.2);
	UIslider* resSlider = new UIslider(vec2(0, 0.3), vec2(0.9, 0.2), vec4(vec3(0.9), 1.0f), vec4(1.0f), "resolution", 0.8);
	UItoggler* framelockToggl = new UItoggler(vec2(0, -0.05), vec2(0.9, 0.1), vec4(vec3(0.9), 1.0f), vec4(1.0f), "unlimited frames",false);
	UItoggler* debugToggl = new UItoggler(vec2(0, -0.05), vec2(0.9, 0.1), vec4(vec3(0.9), 1.0f), vec4(1.0f), "debug lines",true);
	settingsBox->children.push_back(settingsBar);
	settingsBar->children.push_back(graphicsButton);
	settingsBar->children.push_back(dificultyButton);
	graphicsButton->child = graphicsBox;
	menueBox->children.push_back(gridSizeSlider);
	menueBox->children.push_back(enemieSlider);
	menueBox->children.push_back(startGameToggl);
	graphicsBox->children.push_back(brightnessSlider);
	graphicsBox->children.push_back(resSlider);
	dificultyButton->child = dificultyBox;
	dificultyBox->children.push_back(debugToggl);
	graphicsBox->children.push_back(framelockToggl);
	settings = UIsettings();
	settings.brightness = &brightnessSlider->fraction;
	settings.resolution = &resSlider->fraction;
	settings.frameLock = &framelockToggl->isTrue;
	settings.gridSize = &gridSizeSlider->fraction;
	settings.enemies = &enemieSlider->fraction;
	settings.gameStart = &startGameToggl->isTrue;
	settings.debug = &debugToggl->isTrue;
}

void UImenue::draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD) {
	UIelement::draw(VAO, ShaderUI, ps, sc, mouse, mouseD);

	for (int n = 0; n < children.size(); n++) {
		if (dynamic_cast<UIDIV*>(children[n]) != nullptr)(*children[n]).draw(VAO, ShaderUI, pos * sc + ps, sca * sc, mouse, mouseD);
		if (dynamic_cast<UIButton*>(children[n]) != nullptr)
		{
			UIButton* buton = dynamic_cast<UIButton*>(children[n]);
			if (mouseD &&
				mouse.x > (pos.x * sc.x + ps.x + ((*buton).pos.x * sca.x * sc.x)) - (sca.x * sc.x * (*buton).sca.x) &&
				mouse.x < (pos.x * sc.x + ps.x + ((*buton).pos.x * sca.x * sc.x)) + (sca.x * sc.x * (*buton).sca.x) &&
				mouse.y >(pos.y * sc.y + ps.y + ((*buton).pos.y * sca.y * sc.y)) - (sca.y * sc.y * (*buton).sca.y) &&
				mouse.y < (pos.y * sc.y + ps.y + ((*buton).pos.y * sca.y * sc.y)) + (sca.y * sc.y * (*buton).sca.y)

				)
				cur = n;
			if (cur == n && (*buton).child != nullptr)(*buton).child->draw(VAO, ShaderUI, ps, sc, mouse, mouseD);
		}
		(*children[n]).draw(VAO, ShaderUI, pos * sc + ps, sca * sc, mouse, mouseD);
	}
}

UIsettings::UIsettings() {

}