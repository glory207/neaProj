#include "UIelement.h"
#include "texture.h"


GLuint texT;
void UIelement::update(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD) {}
void UIelement::draw(GLuint VAO,GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD) {

	if(texture == -1)glBindTexture(GL_TEXTURE_2D, texT);
	else
	{
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	glUniform1i(glGetUniformLocation(ShaderUI, "chr"), chr);
	glUniform1f(glGetUniformLocation(ShaderUI, "corner"), 0.05);
	glUniform4f(glGetUniformLocation(ShaderUI, "backround"), back.x, back.y, back.z, back.w);
	glUniform4f(glGetUniformLocation(ShaderUI, "textureMatrix"), pos.x * sc.x + ps.x, pos.y * sc.y + ps.y, sca.x * sc.x, sca.y * sc.y);
	glUniform4f(glGetUniformLocation(ShaderUI, "forground"), four.x, four.y, four.z, four.w);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

UIDIV::UIDIV(vec2 sca, vec4 back, vec4 four,bool hrv) {
	this->pos = pos;
	this->sca = sca;
	this->back = back;
	this->four = four;
	this->HorV = hrv;
	chr = -1;

}

void UIDIV::draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD) {
	UIelement::draw(VAO, ShaderUI, ps, sc, mouse, mouseD);

	for (int n = 0; n < children.size(); n ++) if (children[n]->active) {
		children[n]->draw(VAO, ShaderUI, pos * sc + ps, sca * sc, mouse, mouseD);
	}
}

void UIDIV::update(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD) {

		float cumi = 1;
		if (!HorV) cumi = -1;
	for (int n = 0; n < children.size(); n++) if(children[n]->active) {
		if (HorV) {
			cumi -= children[n]->sca.y + 0.05f;
			children[n]->pos.y = cumi - 0.05f;
			cumi -= children[n]->sca.y;
			children[n]->pos.x = 0;
		}
		else {
			cumi += children[n]->sca.x + 0.025f;
			children[n]->pos.x = cumi;
			cumi += children[n]->sca.x;
			children[n]->pos.y = 0;
		}
		children[n]->update(VAO, ShaderUI, pos * sc + ps, sca * sc, mouse, mouseD);
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
		}
	}
	for (int n = 0; n < children.size(); n++) {
		if (dynamic_cast<UIButton*>(children[n]) != nullptr)
		{
			UIButton* buton = dynamic_cast<UIButton*>(children[n]);
			
			(*buton).child->active = false;
			if (cur == n && (*buton).child != nullptr)(*buton).child->active = true;
		}
	}
}

UIButton::UIButton(vec2 sca, vec4 back, vec4 four,string txt) {
	this->pos = pos;
	this->sca = sca;
	this->back = back;
	this->four = four;
	this->text = txt;
	chr = -1;
}
void UIButton::draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD) {
	chr = -1;


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

	glUniform4f(glGetUniformLocation(ShaderUI, "forground"), 0, 0, 0, 1);

	drawString(VAO, ShaderUI, pss, scc, text);
	
}
void UIButton::update(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD) {
	chr = -1;

	hover =
		mouse.x < ((pos.x * sc.x + ps.x) + (sca.x * sc.x)) &&
		mouse.x >((pos.x * sc.x + ps.x) - (sca.x * sc.x)) &&
		mouse.y < ((pos.y * sc.y + ps.y) + (sca.y * sc.y)) &&
		mouse.y >((pos.y * sc.y + ps.y) - (sca.y * sc.y))
		;
}


UIslider::UIslider(vec2 sca, vec4 back, vec4 four, string txt, float frac) {
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

	vec2 off = vec2(0);
	if (hover && mouseD) {
		off = vec2(0.005,0.01);
	}
	else if (hover) {
		off = vec2(0.01);
	}

	UIelement::draw(VAO, ShaderUI, ps , sc, mouse, mouseD);

	glUniform1f(glGetUniformLocation(ShaderUI, "corner"), 0.02);
	glUniform4f(glGetUniformLocation(ShaderUI, "backround"), 1, 1, 0, 1);
	glUniform4f(glGetUniformLocation(ShaderUI, "forground"), 1, 1, 0, 1);
	glUniform1i(glGetUniformLocation(ShaderUI, "chr"), -1);
	glUniform4f(glGetUniformLocation(ShaderUI, "textureMatrix"), pos.x* sc.x + ps.x + (sca.x * sc.x*0.9 * (fraction-1.0)),-0.4 * sca.y * sc.y + pos.y* sc.y + ps.y, sca.x * sc.x* (fraction)*0.9, sca.y* sc.y * 0.4);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	glUniform4f(glGetUniformLocation(ShaderUI, "forground"), 0, 0, 0, 1);

	string t = text + " " + to_string(int(fraction * 100)) + "%";
	drawString(VAO, ShaderUI, vec2(0, 0.75) * vec2(1, 0.5) * (sca * sc) + (pos * sc + ps ),vec2(1,0.5) * (sca * sc), t);
}

void UIslider::update(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD) {
	chr = -1;
	hover =
		mouse.x < ((pos.x * sc.x + ps.x) + (sca.x * sc.x)) &&
		mouse.x >((pos.x * sc.x + ps.x) - (sca.x * sc.x)) &&
		mouse.y < ((pos.y * sc.y + ps.y) + (sca.y * sc.y)) &&
		mouse.y >((pos.y * sc.y + ps.y) - (sca.y * sc.y));

	if (hover && mouseD) {
		fraction = (mouse.x-((pos.x * sc.x + ps.x) - (sca.x * sc.x)*0.9)) / ((sca.x * sc.x) * 2.0*0.9);
		if (fraction < 0)fraction = 0;
		if (fraction > 1)fraction = 1;
	}

}

UItoggler::UItoggler(vec2 sca, vec4 back, vec4 four, string txt, bool tr,bool bg) {
	this->pos = pos;
	this->sca = sca;
	this->back = back;
	this->four = four;
	this->text = txt;
	this->isTrue = tr;
	this->big = bg; 
	chr = -1; 
}

void UItoggler::draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD) {
	chr = -1;


	vec2 poss = pos * sc + ps ;
	vec2 scaa = sca * sc;

	vec2 oss = vec2(0.75,0.0);
	vec2 caa = vec2(0.15, 0.8);

	vec2 off = vec2(0);

	if (isTrue) {
		if (hover && mouseD) {
			off -= vec2(0, 0.01);
		}
		else if (hover) {
			off += vec2(0, 0.01);
		}
		off -= vec2(0, 0.0025);
	}
	else {
		if (hover && mouseD) {
			off -= vec2(0, 0.01);
		}
		else if (hover) {
			off += vec2(0, 0.02);
		}
		off += vec2(0, 0.004);
	}
	poss += off;

	if (big) {
		UIelement::draw(VAO, ShaderUI, ps + off, sc, mouse, mouseD);

		glUniform4f(glGetUniformLocation(ShaderUI, "forground"), 0, 0, 0, 1);
		drawString(VAO, ShaderUI,vec2(0.5f,0) * (sca * sc) + (pos * sc + ps) + off, (sca * sc), text);
	}
	else {
		UIelement::draw(VAO, ShaderUI, ps, sc, mouse, mouseD);
	glUniform1i(glGetUniformLocation(ShaderUI, "chr"), chr); 
	glUniform1f(glGetUniformLocation(ShaderUI, "corner"), 0.05); 

	
	glUniform4f(glGetUniformLocation(ShaderUI, "textureMatrix"), oss.x * scaa.x + poss.x, oss.y * scaa.y + poss.y, caa.x * scaa.x, caa.y * scaa.y);
	glUniform4f(glGetUniformLocation(ShaderUI, "backround"), back.x*0.9, back.y*0.9, back.z*0.9, 1);
	glBindVertexArray(VAO); 
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
	

	caa *= 0.6;

	glUniform1f(glGetUniformLocation(ShaderUI, "corner"), 0.025);
	glUniform4f(glGetUniformLocation(ShaderUI, "textureMatrix"), oss.x * scaa.x + poss.x, oss.y * scaa.y + poss.y, caa.x * scaa.x, caa.y * scaa.y);
	if (isTrue)glUniform4f(glGetUniformLocation(ShaderUI, "backround"), 1, 1, 0, 1);
	else glUniform4f(glGetUniformLocation(ShaderUI, "backround"), back.x, back.y, back.z, 1);

	glUniform4f(glGetUniformLocation(ShaderUI, "forground"), 0,0,0,1);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	drawString(VAO, ShaderUI, vec2(-0.3, 0.0) * vec2(0.7, 0.5) * (sca * sc) + (pos * sc + ps), vec2(0.7, 0.5) * (sca * sc), text);
	}
	
}

void UItoggler::update(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD) {
	chr = -1;
	vec2 poss = pos * sc + ps;
	vec2 scaa = sca * sc;

	vec2 oss = vec2(0.75,0.0);
	vec2 caa = vec2(0.15, 0.8);
	if (big) {
		hover =
			mouse.x < ((pos.x * sc.x + ps.x) + (sca.x * sc.x)) &&
			mouse.x >((pos.x * sc.x + ps.x) - (sca.x * sc.x)) &&
			mouse.y < ((pos.y * sc.y + ps.y) + (sca.y * sc.y)) &&
			mouse.y >((pos.y * sc.y + ps.y) - (sca.y * sc.y));
	}
	else {
		hover =
			mouse.x < ((oss.x * scaa.x + poss.x) + (caa.x * scaa.x)) &&
			mouse.x >((oss.x * scaa.x + poss.x) - (caa.x * scaa.x)) &&
			mouse.y < ((oss.y * scaa.y + poss.y) + (caa.y * scaa.y)) &&
			mouse.y >((oss.y * scaa.y + poss.y) - (caa.y * scaa.y));
	}
	

	if ((hover && mouseD) && !isDown) {
		isTrue = !isTrue;
	}
	if (hover && mouseD) isDown = true; 
	else isDown = false;

}

/*
void drawString(GLuint VAO, GLuint ShaderUI, vec2 pss, vec2 scc,string text) {
	// sets the size of a single character so all of them fit into the UI
	vec2 positionOfChar = vec2(0);
	int sizeOfChar = text.size();
	vec2 sa = vec2(1.0 / sizeOfChar, 1.0);
	glUniform4f(glGetUniformLocation(ShaderUI, "backround"), 0, 0, 0, 0);
	// cycles through all the characters to be drawn
	for (int i = 0; i < text.size(); i++)
	{
		// translates the ASCII value into positions on the texture
		if (tolower(int(text[i])) >= 97 && tolower(int(text[i])) <= 122)glUniform1i(glGetUniformLocation(ShaderUI, "chr"), tolower(int(text[i])) - 87);
		else if (tolower(int(text[i])) >= 48 && int(text[i]) <= 57)glUniform1i(glGetUniformLocation(ShaderUI, "chr"), tolower(int(text[i])) - 48);
		else if (tolower(int(text[i])) == 45)glUniform1i(glGetUniformLocation(ShaderUI, "chr"), -7);
		else if (tolower(int(text[i])) == 61)glUniform1i(glGetUniformLocation(ShaderUI, "chr"), -6);
		else if (tolower(int(text[i])) == 46)glUniform1i(glGetUniformLocation(ShaderUI, "chr"), -5);
		else if (tolower(int(text[i])) == 37)glUniform1i(glGetUniformLocation(ShaderUI, "chr"), -4);
		else if (tolower(int(text[i])) == 44)glUniform1i(glGetUniformLocation(ShaderUI, "chr"), -3);
		else if (tolower(int(text[i])) == 33)glUniform1i(glGetUniformLocation(ShaderUI, "chr"), -2);
		else glUniform1i(glGetUniformLocation(ShaderUI, "chr"), -1);
		positionOfChar.x = (1.0 + i * 2) / float(sizeOfChar) - 1;
		// draws the character
		glUniform4f(glGetUniformLocation(ShaderUI, "textureMatrix"), positionOfChar.x * scc.x + pss.x, positionOfChar.y * scc.y + pss.y, sa.x * scc.x, sa.y * scc.y);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}
*/
//*
void drawString(GLuint VAO, GLuint ShaderUI, vec2 pss, vec2 scc,string text) {
	vec2 positionOfChar = vec2(0);
	int sizeOfChar = text.size() + 3;
	vec2 sa = vec2(0.9 / sizeOfChar, 0.5);
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
		else glUniform1i(glGetUniformLocation(ShaderUI, "chr"), 42);
		positionOfChar.x = (2.0 + i * 2) / float(sizeOfChar) - 1;
		glUniform4f(glGetUniformLocation(ShaderUI, "textureMatrix"), positionOfChar.x * scc.x + pss.x, positionOfChar.y * scc.y + pss.y, sa.x * scc.x, sa.y * scc.y);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}
//*/

UImenue::UImenue(GLuint teT) {


	texT = teT;

	pos = vec2(0);
	sca = vec2(0.95);
	back = vec4(vec3(0.6), 1.0f);
	four = vec4(1.0f);
	chr = -1;

	vec4 col1 = vec4(vec3(0.88f), 1.0f);
	vec4 col2 = vec4(vec3(172, 184, 192) / 255.0f, 1.0f);

	fullBox = new UIDIV(vec2(0.9f), col1, vec4(1.0f), true);

	//	top bar
	UIDIV* topBar = new UIDIV(vec2(0.85f, 0.1), col2, vec4(1.0f), false);
	fullBox->children.push_back(topBar);

	UItoggler* close = new UItoggler(vec2(0.075f, 0.7), vec4(1,1,0,1), vec4(1.0f), "X", false, true);
	topBar->children.push_back(close);

	//		main
	UIButton* mainButton = new UIButton(vec2(0.15f, 0.7), col1, vec4(1.0f), "main");
	topBar->children.push_back(mainButton);
	UIDIV* mainBox = new UIDIV(vec2(0.95f, 0.8), col2, vec4(1.0f), true);
	mainButton->child = mainBox;
	fullBox->children.push_back(mainBox);
	//				box
	UIDIV* boxTop = new UIDIV(vec2(0.9, 0.6), col1, vec4(1.0f), false);
	mainBox->children.push_back(boxTop);
	//				box
	UIDIV* boxBot = new UIDIV(vec2(0.9, 0.3), col1, vec4(1.0f), true);
	mainBox->children.push_back(boxBot);
	//				box
	UIDIV* boxTopL = new UIDIV(vec2(0.4, 0.8), col2, vec4(1.0f), true);
	boxTop->children.push_back(boxTopL);
	//				box
	UIDIV* boxTopR = new UIDIV(vec2(0.55, 0.9), col2, vec4(1.0f), false);
	boxTopR->chr = 85;
	boxTop->children.push_back(boxTopR);
	//				map size
	UIslider* mapMizeSlider = new UIslider(vec2(0.9, 0.325), col1, vec4(1.0f), "map size    ", 0.2);
	boxTopL->children.push_back(mapMizeSlider);
	//				difficulty
	UIslider* difficultySlider = new UIslider(vec2(0.9, 0.325), col1, vec4(1.0f), "difficulty   ", 0.2);
	boxTopL->children.push_back(difficultySlider);
	//				create
	UItoggler* generate = new UItoggler(vec2(0.8f, 0.2f), col1, vec4(1.0f), "generate     ", true, true);
	boxTopL->children.push_back(generate);


	//				start
	UItoggler* mainStart = new UItoggler(vec2(0.8f, 0.45f), col2, vec4(1.0f), "start game     ", false, true);
	boxBot->children.push_back(mainStart);
	//				quit
	UItoggler* mainQuit = new UItoggler(vec2(0.7f, 0.4f), col2, vec4(1.0f), "quit to game     ", false, true);
	boxBot->children.push_back(mainQuit);




	//		pause
	UIButton* pauseButton = new UIButton(vec2(0.15f, 0.7), col1, vec4(1.0f), "pause");
	topBar->children.push_back(pauseButton);
	UIDIV* pauseBox = new UIDIV(vec2(0.95f, 0.8), col2, vec4(1.0f), true);
	pauseButton->child = pauseBox;
	fullBox->children.push_back(pauseBox);
	//				box
	UIDIV* box = new UIDIV(vec2(0.5, 0.55), col1, vec4(1.0f), true);
	box->chr = 85;
	pauseBox->children.push_back(box);
	//				master volume
	UIslider* masterVolumeSlider = new UIslider(vec2(0.9, 0.15), col1, vec4(1.0f), "master volume          ", 0.2);
	pauseBox->children.push_back(masterVolumeSlider);
	//				quit
	UItoggler* pauseQuit = new UItoggler(vec2(0.7f, 0.1f), col1, vec4(1.0f), "quit to main     ", false, true);
	pauseBox->children.push_back(pauseQuit);





	//		settings
	UIButton* settingsButton = new UIButton(vec2(0.15f, 0.7), col1, vec4(1.0f), "settings");
	topBar->children.push_back(settingsButton);
	UIDIV* settingsBox = new UIDIV(vec2(0.95f, 0.8), col2, vec4(1.0f), false);
	settingsButton->child = settingsBox;
	fullBox->children.push_back(settingsBox);
	//UIDIV* settingsBar = new UIDIV(vec2(0.35f, 0.9f), col1, vec4(1.0f), true);
	UIDIV* settingsBar = new UIDIV(vec2(0.2f, 0.9f), col1, vec4(1.0f), true);
	settingsBox->children.push_back(settingsBar);
	//			graphics
	UIButton* graphicsButton = new UIButton(vec2(0.9f, 0.09f), col2, vec4(1.0f), "graphics");
	settingsBar->children.push_back(graphicsButton);
	UIDIV* graphicsBox = new UIDIV(vec2(0.575f, 0.9), col1, vec4(1.0f), true);
	graphicsButton->child = graphicsBox;
	settingsBox->children.push_back(graphicsBox);
	//				brightness
	UIslider* brightnessSlider = new UIslider(vec2(0.9, 0.2), col2, vec4(1.0f), "brightness", 0.2);
	graphicsBox->children.push_back(brightnessSlider);
	//				resolution
	UIslider* resolutionSlider = new UIslider(vec2(0.9, 0.2), col2, vec4(1.0f), "resolution", 1);
	graphicsBox->children.push_back(resolutionSlider);
	//				frame limit
	UItoggler* frameLimitToggl = new UItoggler(vec2(0.9, 0.1), col2, vec4(1.0f), "frame limit", false, false);
	graphicsBox->children.push_back(frameLimitToggl);
	//			audio
	UIButton* audioButton = new UIButton(vec2(0.9f, 0.09f), col2, vec4(1.0f), "audio");
	settingsBar->children.push_back(audioButton);
	UIDIV* audioBox = new UIDIV(vec2(0.575f, 0.9), col1, vec4(1.0f), true);
	audioButton->child = audioBox;
	settingsBox->children.push_back(audioBox);
	//				music volume
	UIslider* musicVolumeSlider = new UIslider(vec2(0.9, 0.15), col2, vec4(1.0f), "music volume          ", 0.2);
	audioBox->children.push_back(musicVolumeSlider);
	//				player volume
	UIslider* playerVolumeSlider = new UIslider(vec2(0.9, 0.15), col2, vec4(1.0f), "player volume          ", 0.2);
	audioBox->children.push_back(playerVolumeSlider);
	//				enemy volume
	UIslider* enemyVolumeSlider = new UIslider(vec2(0.9, 0.15), col2, vec4(1.0f), "enemy volume          ", 0.2);
	audioBox->children.push_back(enemyVolumeSlider);
	//				other volume
	UIslider* otherVolumeSlider = new UIslider(vec2(0.9, 0.15), col2, vec4(1.0f), "other volume          ", 0.2);
	audioBox->children.push_back(otherVolumeSlider);
	//			controls
	UIButton* controlsButton = new UIButton(vec2(0.9f, 0.09f), col2, vec4(1.0f), "controls");
	settingsBar->children.push_back(controlsButton);
	UIDIV* controlsBox = new UIDIV(vec2(0.575f, 0.9), col1, vec4(1.0f), true);
	controlsButton->child = controlsBox;
	settingsBox->children.push_back(controlsBox);
	//				mouse sensitivity
	UIslider* mouseSensitivitySlider = new UIslider(vec2(0.9, 0.2), col2, vec4(1.0f), "mouse sensitivity", 0.2);
	controlsBox->children.push_back(mouseSensitivitySlider);
	//				controller  sensitivity
	UIslider* controllerSensitivitySlider = new UIslider(vec2(0.9, 0.2), col2, vec4(1.0f), "controller sensitivity", 0.2);
	controlsBox->children.push_back(controllerSensitivitySlider);
	//			debug
	UIButton* debugButton = new UIButton(vec2(0.9f, 0.09f), col2, vec4(1.0f), "debug");
	settingsBar->children.push_back(debugButton);
	UIDIV* debugBox = new UIDIV(vec2(0.575f, 0.9), col1, vec4(1.0f), true);
	debugButton->child = debugBox;
	settingsBox->children.push_back(debugBox);
	//				debug
	UItoggler* debugToggl = new UItoggler(vec2(0.9, 0.125), col2, vec4(1.0f), "debug", false, false);
	debugBox->children.push_back(debugToggl);



	settings = UIsettings();
	// settings
	//		graphics
	settings.resolution = &resolutionSlider->fraction;
	settings.brightness = &brightnessSlider->fraction;
	settings.frameLock = &frameLimitToggl->isTrue;
	//		audio
	settings.masterVolume = &masterVolumeSlider->fraction;
	settings.musicVolume = &musicVolumeSlider->fraction;
	settings.playerVolume = &playerVolumeSlider->fraction;
	settings.enemyVolume = &enemyVolumeSlider->fraction;
	settings.otherVolume = &otherVolumeSlider->fraction;
	//		controls
	settings.mouseSensitivity = &mouseSensitivitySlider->fraction;
	settings.controllerSensitivity = &controllerSensitivitySlider->fraction;
	//		debug
	settings.debug = &debugToggl->isTrue;

	// pause
	settings.quitToMain = &pauseQuit->isTrue;

	// main
	settings.gridSize = &mapMizeSlider->fraction;
	settings.enemies = &difficultySlider->fraction;
	settings.gameStart = &mainStart->isTrue;
	settings.quitGame = &mainQuit->isTrue;
	settings.close = &close->isTrue;
	settings.generate = &generate->isTrue;


}

void UImenue::draw(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD) {
	UIelement::draw(VAO, ShaderUI, ps, sc, mouse, mouseD);
	fullBox->draw(VAO, ShaderUI, pos * sc + ps, sca * sc, mouse, mouseD);
}

void UImenue::update(GLuint VAO, GLuint ShaderUI, vec2 ps, vec2 sc, vec2 mouse, bool mouseD) {
	fullBox->update(VAO, ShaderUI, pos * sc + ps, sca * sc, mouse, mouseD);
}

UIsettings::UIsettings() {

}