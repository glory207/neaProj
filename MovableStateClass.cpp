#include "MovableStateClass.h"
 
MovableState::MovableState(InputObject* inp, StateClass* sender) {
	this->inp = inp;
	idle = new IdleState(this->inp, this);
	motion = new MotionState(this->inp, this);
	inAir = new InAirState(this->inp, this);
	cur = idle;

}
int MovableState::Enter() {
	
	//cout << "MovableState" << endl;
	cur->Enter();
	return 0;
}
int MovableState::update(float deltaTime) {
	////cout << (*inp).inp.x << " " << inp->inp.y << endl;
	if (!inp->Grounded && cur != inAir)
	{
		cur = inAir;
		cur->Enter();
	}
	cur->update(deltaTime);


	int dir = floor(((
		-atan2(inp->pos.x - inp->cam.x, -inp->pos.z + inp->cam.z)
		+ atan2(inp->acc.x, -inp->acc.z)
		+ 0.125 * 3.141592) / (0.25 * 3.141592)));
	if (dir != NULL) inp->dir[0] = dir;

	inp->acc.z -= inp->acc.z * inp->resistance.z * deltaTime;
	inp->acc.y -= inp->acc.y * inp->resistance.y * deltaTime;
	inp->acc.x -= inp->acc.x * inp->resistance.x * deltaTime;

	inp->pos.z -= inp->acc.z * deltaTime;
	inp->pos.y += inp->acc.y * deltaTime;
	inp->pos.x -= inp->acc.x * deltaTime;
	return 0;
}
void MovableState::Exit() {}


IdleState::IdleState(InputObject* inp, MovableState* sender) {
	this->sender = sender;
	this->inp = inp;
	stood = new IdleStoodState(inp,this);
	crouched = new IdleCrouchState(inp,this);
	cur = stood;
}
int IdleState::Enter() {
	//cout << "IdleState" << endl;
	if (inp->SP) {
		cur = crouched;
	}
	else
	{
		cur = stood;
	}
	cur->Enter();
	return 0;
}
int IdleState::update(float deltaTime) {
	if (length(inp->inp) > 0.05f){
		sender->cur = sender->motion;
		sender->cur->Enter(); return 0;
	}
	cur->update(deltaTime);
	return 0;
}
void IdleState::Exit() {}

IdleStoodState::IdleStoodState(InputObject* inp, IdleState* sender) {
	this->sender = sender;
	this->inp = inp;
}
int IdleStoodState::Enter() {
	//cout << "IdleStoodState" << endl;

	inp->animation[0] = 2;
	inp->animation[1] = 7;
	inp->animation[2] = 7;
	inp->animation[3] = 7.0;
	return 0;
}
int IdleStoodState::update(float deltaTime) {
	if (inp->SP) {
		sender->cur = sender->crouched;
		sender->cur->Enter(); return 0;
	}

	return 0;
}
void IdleStoodState::Exit() {}

IdleCrouchState::IdleCrouchState(InputObject* inp, IdleState* sender) {
	this->sender = sender;
	this->inp = inp;
}
int IdleCrouchState::Enter() {
	//cout << "IdleCrouchState" << endl;

	inp->animation[0] = 17;
	inp->animation[1] = 18;
	inp->animation[2] = 15;
	inp->animation[3] = 4.0;
	return 0;
}
int IdleCrouchState::update(float deltaTime) {
	if (!inp->SP) {
		sender->cur = sender->stood;
		sender->cur->Enter(); return 0;
	}

	return 0;
}
void IdleCrouchState::Exit() {}






MotionState::MotionState(InputObject* inp, MovableState* sender) {
	this->sender = sender;
	this->inp = inp;
	walk = new WalkState(inp, this);
	run = new RunState(inp, this);
	crawl = new CrawlState(inp, this);
	cur = walk;
}
int MotionState::Enter() {
	//cout << "MotionState" << endl;
	if (length(inp->inp) < 0.05f) {
		sender->cur = sender->idle;
		sender->cur->Enter(); return 0;
	}
	inp->speed = 5.0;
	cur->Enter();
	return 0;
}
int MotionState::update(float deltaTime) {
	if (length(inp->inp) < 0.05f) {
		sender->cur = sender->idle;
		sender->cur->Enter();return 0;
	}
	cur->update(deltaTime);
	vec2 mov = vec2((inp->inp.y * sin(inp->rot.y) - inp->inp.x * cos(inp->rot.y)), (inp->inp.y * cos(inp->rot.y) + inp->inp.x * sin(inp->rot.y)));


	inp->acc.z += normalize(mov).y * deltaTime * inp->speed;
	inp->acc.x += normalize(mov).x * deltaTime * inp->speed;
	return 0;
}
void MotionState::Exit() {}


WalkState::WalkState(InputObject* inp, MotionState* sender) {
	this->sender = sender;
	this->inp = inp;
}
int WalkState::Enter() {

	//cout << "WalkState" << endl;
	if (inp->SP) {
		sender->cur = sender->crawl;
		sender->cur->Enter(); return 0;
	}
	if (inp->SH) {
		sender->cur = sender->run;
		sender->cur->Enter(); return 0;
	}
	inp->animation[0] = 1;
	inp->animation[1] = 6;
	inp->animation[2] = 10;
	inp->animation[3] = 8.0;
	inp->speed = 3.0;
	return 0;
}
int WalkState::update(float deltaTime) {
	if (inp->SP) {
		sender->cur = sender->crawl;
		sender->cur->Enter(); return 0;
	}
	if (inp->SH) {
		sender->cur = sender->run;
		sender->cur->Enter(); return 0;
	}
	return 0;
}
void WalkState::Exit() {}


RunState::RunState(InputObject* inp, MotionState* sender) {
	this->sender = sender;
	this->inp = inp;
}
int RunState::Enter() {

	//cout << "RunState" << endl;
	if (!inp->SH) {
		sender->cur = sender->walk;
		sender->cur->Enter(); return 0;
	}
	if (inp->SP) {
		sender->sender->cur = sender->sender->inAir;
		sender->sender->cur->Enter();
		return 0;
	}
	inp->animation[0] = 8;
	inp->animation[1] = 9;
	inp->animation[2] = 9;
	inp->animation[3] = 10.0;
	inp->speed = 6.0;
	inp->jump = vec2(1.6,2.0);
	return 0;
}
int RunState::update(float deltaTime) {
	
	if (!inp->SH) {
		sender->cur = sender->walk;
		sender->cur->Enter(); return 0;
	}
	if (inp->SP) {
		sender->sender->cur = sender->sender->inAir;
		sender->sender->cur->Enter();
		return 0;
	}
	return 0;
}
void RunState::Exit() {}


CrawlState::CrawlState(InputObject* inp, MotionState* sender) {
	this->sender = sender;
	this->inp = inp;
}
int CrawlState::Enter() {

	//cout << "CrawlState" << endl;
	if (!inp->SP) {
		sender->cur = sender->walk;
		sender->cur->Enter(); return 0;
	}
	if (inp->SH) {
		sender->sender->cur = sender->sender->inAir;
		sender->sender->cur->Enter();
		return 0;
	}
	inp->animation[0] = 3;
	inp->animation[1] = 4;
	inp->animation[2] = 11;
	inp->animation[3] = 7.0;
	inp->speed = 1.0;
	inp->jump = vec2(0.7f, 1.5f);
	return 0;

}
int CrawlState::update(float deltaTime) {
	if (!inp->SP) {
		sender->cur = sender->walk;
		sender->cur->Enter(); return 0;
	}
	if (inp->SH) {
		sender->sender->cur = sender->sender->inAir;
		sender->sender->cur->Enter();
		return 0;
	}
	return 0;
}
void CrawlState::Exit() {}





InAirState::InAirState(InputObject* inp, MovableState* sender) {
	this->sender = sender;
	this->inp = inp;
	dive = new DiveState(inp, this);
	roll = new RollState(inp, this);
	fall = new FallingState(inp, this);
	cur = fall;
}
int InAirState::Enter() {
	//cout << "InAirState" << endl;

	inp->resistance = vec3(1,1,1);
	if (inp->SH && inp->SP) {
		cur = dive;
	}
	else
	{
		cur = fall;
	}
	cur->Enter();
	return 0;
}
int InAirState::update(float deltaTime) {
	if (inp->Grounded && cur != roll) {
		cur = roll;
		cur->Enter();
		return 0;
	}
	inp->acc.y -= 5.5f * deltaTime;
	cur->update(deltaTime);
	return 0;
}
void InAirState::Exit() {
	if (inp->inp == vec2(0)) sender->cur = sender->idle;
	else sender->cur = sender->motion;
	inp->resistance = vec3(7);
	sender->cur->Enter();
}

DiveState::DiveState(InputObject* inp, InAirState* sender) {
	this->sender = sender;
	this->inp = inp;
}
int DiveState::Enter() {

	//cout << "DiveState" << endl;

	inp->animation[0] = 21;
	inp->animation[1] = 22;
	inp->animation[2] = 8;
	inp->animation[3] = 10.0;
	inp->animation[4] = 0.0;
	inp->acc.y = inp->jump.x;

	vec2 mov = vec2((inp->inp.y * sin(inp->rot.y) - inp->inp.x * cos(inp->rot.y)), (inp->inp.y * cos(inp->rot.y) + inp->inp.x * sin(inp->rot.y)));
	inp->acc.x = normalize(mov).x * inp->jump.y;
	inp->acc.z = normalize(mov).y * inp->jump.y;
	return 0;
}
int DiveState::update(float deltaTime) {
	if (inp->animation[4] <= 3.0f) {
		inp->animation[3] = 20.0f;
	}
	if (inp->animation[4] >= 7.0f) {
		inp->animation[4] = 3.0f;
	}
	return 0;
}
void DiveState::Exit() {}


RollState::RollState(InputObject* inp, InAirState* sender) {
	this->sender = sender;
	this->inp = inp;
}
int RollState::Enter() {

	//cout << "RollState" << endl;
	inp->acc.y = -0.1f;
	if (length(inp->acc) < 0.5f)
	{
		sender->Exit();
		return 0;
	}
	else
	{

		inp->animation[0] = 19;
		inp->animation[1] = 20;
		inp->animation[2] = 6;
		inp->animation[3] = 10.0;
		inp->animation[4] = 0.0;

	}

	return 0;
}
int RollState::update(float deltaTime) {
	inp->acc.y = -1.1f;
	vec2 mov = vec2((inp->inp.y * sin(inp->rot.y) - inp->inp.x * cos(inp->rot.y)), (inp->inp.y * cos(inp->rot.y) + inp->inp.x * sin(inp->rot.y)));

	if (dot(mov, vec2(inp->acc.x, inp->acc.z)) > 0.5f)
	{
		inp->resistance.x = 1.0f;
		inp->resistance.z = 1.0f;
	}
	else
	{
		inp->resistance.x = 7.0f;
		inp->resistance.z = 7.0f;
	}
	if (inp->animation[4] >= 5.0) {
		sender->Exit();
	}
	return 0;
}
void RollState::Exit() {}


FallingState::FallingState(InputObject* inp, InAirState* sender) {
	this->sender = sender;
	this->inp = inp;
}
int FallingState::Enter() {

	//cout << "FallingState" << endl;

	return 0;
}
int FallingState::update(float deltaTime) {
	
	return 0;
}
void FallingState::Exit() {}





/*
stateName::stateName(InputObject* inp, StateClass* sender) {}
int stateName::Enter() {}
int stateName::update(float deltaTime) {}
void stateName::Exit() {}
*/