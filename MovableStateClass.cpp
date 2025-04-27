#include "MovableStateClass.h"
 
MovableState::MovableState(InputObject* inp, StateClass* sender) {
	this->inp = inp;
	idle = new IdleState(this->inp, this);
	motion = new MotionState(this->inp, this);
	inAir = new InAirState(this->inp, this);
	cur = idle;

}
int MovableState::Enter() {
	
	// cout << "MovableState" << endl;
	cur->Enter();
	return 0;
}
int MovableState::update(float deltaTime) {
	// change states
	if (!inp->Grounded && cur != inAir)
	{
		cur = inAir;
		cur->Enter();
	}
	// update the base state
	cur->update(deltaTime);

	// preform logic


	if (!inp->WH) {

		inp->cam->rot.x += inp->lok.y;
		inp->cam->rot.y += inp->lok.x;
		inp->rot.y = inp->cam->rot.y;
	}
	 

	inp->camArm = inp->camArm + (inp->camArmTarget - inp->camArm) * deltaTime * 10.0f;


	// validation stops the inp->camera from rotation 
	// upside down 
	if (inp->cam->rot.x > 0.4f)inp->cam->rot.x = 0.4f;
	if (inp->cam->rot.x < -1.15f)inp->cam->rot.x = -1.15f;

	inp->cam->pos.x = inp->pos.x + sin(inp->cam->rot.y) * inp->camArm.x * cos(inp->cam->rot.x) + cos(inp->cam->rot.y) * inp->camArm.y;
	inp->cam->pos.y = inp->pos.y + inp->camArm.z - sin(inp->cam->rot.x) * inp->camArm.x;
	inp->cam->pos.z = inp->pos.z + cos(inp->cam->rot.y) * inp->camArm.x * cos(inp->cam->rot.x) - sin(inp->cam->rot.y) * inp->camArm.y;


	if (inp->WH && length(inp->lok) > 0.02) {
		inp->whe = normalize(vec2(-inp->lok.x, inp->lok.y));
	}
	inp->lok = vec2(0);

	int dir = floor(((
		-atan2(inp->pos.x - inp->cam->pos.x, -inp->pos.z + inp->cam->pos.z)
		+ atan2(inp->vel.x, -inp->vel.z)
		+ 0.125 * 3.141592) / (0.25 * 3.141592)));
	if (dir != NULL) inp->animation.dir = dir;

	inp->vel.z -= inp->vel.z * inp->resistance.z * deltaTime;
	inp->vel.y -= inp->vel.y * inp->resistance.y * deltaTime;
	inp->vel.x -= inp->vel.x * inp->resistance.x * deltaTime;

	inp->pos.z -= inp->vel.z * deltaTime;
	inp->pos.y += inp->vel.y * deltaTime;
	inp->pos.x -= inp->vel.x * deltaTime;
	return 0;
}


IdleState::IdleState(InputObject* inp, MovableState* sender) {
	this->sender = sender;
	this->inp = inp;
	stood = new IdleStoodState(inp,this);
	crouched = new IdleCrouchState(inp,this);
	cur = stood;
}
int IdleState::Enter() {
	// cout << "IdleState" << endl;
	if (inp->SP) {
		cur = crouched;
	}
	else
	{
		cur = stood;
	}
	inp->resistance = vec3(7);
	cur->Enter();
	return 0;
}
int IdleState::update(float deltaTime) {
	// cout << "IdleState" << endl;
	if (length(inp->inp) > 0.05f){
		sender->cur = sender->motion;
		sender->cur->Enter(); return 0;
	}
	cur->update(deltaTime);
	return 0;
}

IdleStoodState::IdleStoodState(InputObject* inp, IdleState* sender) {
	this->sender = sender;
	this->inp = inp;
}
int IdleStoodState::Enter() {
	// cout << "IdleStoodState" << endl;
	inp->animation = AnimationClass(2,7,7,0,1);
	inp->animation.colour     = 2;
	inp->animation.normal     = 7;
	inp->animation.framecount = 7;
	inp->animation.fps        = 7.0;
	inp->visible = 1;
	inp->camArmTarget = vec3(2.57f, 0.25f, 1.55f) * inp->obj.sca.y;
	return 0;
}
int IdleStoodState::update(float deltaTime) {
	if (inp->SP) {
		sender->cur = sender->crouched;
		sender->cur->Enter(); return 0;
	}

	return 0;
}

IdleCrouchState::IdleCrouchState(InputObject* inp, IdleState* sender) {
	this->sender = sender;
	this->inp = inp;
}
int IdleCrouchState::Enter() {
	// cout << "IdleCrouchState" << endl;
	inp->animation.colour     = 17;
	inp->animation.normal     = 18;
	inp->animation.framecount = 15;
	inp->animation.fps        = 4.0;
	// how visible the state makes you
	inp->visible = 0.5f;
	inp->camArmTarget = vec3(1.37f, 0.25f, 0.95) * inp->obj.sca.y;
	return 0;
}
int IdleCrouchState::update(float deltaTime) {
	// cout << "IdleCrouchState" << endl;
	if (!inp->SP) {
		sender->cur = sender->stood;
		sender->cur->Enter(); return 0;
	}

	return 0;
}






MotionState::MotionState(InputObject* inp, MovableState* sender) {
	this->sender = sender;
	this->inp = inp;
	walk = new WalkState(inp, this);
	run = new RunState(inp, this);
	crawl = new CrawlState(inp, this);
	cur = walk;
}
int MotionState::Enter() {
	inp->resistance = vec3(7);
	if (length(inp->inp) < 0.05f) {
		sender->cur = sender->idle;
		sender->cur->Enter();return 0;
	}
	inp->speed = 5.0;
	cur->Enter();
	return 0;
}
int MotionState::update(float deltaTime) {
	// cout << "MotionState" << endl;
	if (length(inp->inp) < 0.05f) {
		sender->cur = sender->idle;
		sender->cur->Enter(); return 0;
	}
	cur->update(deltaTime);
	vec2 mov = vec2((inp->inp.y * sin(inp->rot.y) - inp->inp.x * cos(inp->rot.y)), (inp->inp.y * cos(inp->rot.y) + inp->inp.x * sin(inp->rot.y)));


	inp->vel.z += normalize(mov).y * deltaTime * inp->speed;
	inp->vel.x += normalize(mov).x * deltaTime * inp->speed;
	return 0;
}


WalkState::WalkState(InputObject* inp, MotionState* sender) {
	this->sender = sender;
	this->inp = inp;
}

int WalkState::Enter() {
	// cout << "WalkState" << endl;
	// pre check the state
	if (inp->SP) {
		sender->cur = sender->crawl;
		sender->cur->Enter();return 0;
	}
	if (inp->SH) {
		sender->cur = sender->run;
		sender->cur->Enter();return 0;
	}
	// then apply animation
	inp->animation.colour     = 1;
	inp->animation.normal     = 6;
	inp->animation.framecount = 10;
	inp->animation.fps        = 8.0;
	inp->speed = 3.0;
	inp->visible = 2;
	inp->camArmTarget = vec3(2.57f, 0.25f, 1.55f) * inp->obj.sca.y;
	return 0;
}
int WalkState::update(float deltaTime) {
	// cout << "WalkState" << endl;
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


RunState::RunState(InputObject* inp, MotionState* sender) {
	this->sender = sender;
	this->inp = inp;
}
int RunState::Enter() {

	// cout << "RunState" << endl;
	if (!inp->SH) {
		sender->cur = sender->walk;
		sender->cur->Enter();return 0;
	}
	if (inp->SP) {
		sender->sender->cur = sender->sender->inAir;
		sender->sender->cur->Enter();
		return 0;
	}
	inp->animation.colour     = 8;
	inp->animation.normal     = 9;
	inp->animation.framecount = 9;
	inp->animation.fps        = 10.0;
	inp->speed = 6.0;
	inp->jump = vec2(1.6,2.0);
	inp->visible = 3;
	inp->camArmTarget = vec3(2.57f, 0.25f, 1.55f) * inp->obj.sca.y;
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


CrawlState::CrawlState(InputObject* inp, MotionState* sender) {
	this->sender = sender;
	this->inp = inp;
}
int CrawlState::Enter() {
	// cout << "CrawlState" << endl;
	if (!inp->SP) {
		sender->cur = sender->walk;
		sender->cur->Enter();return 0;
	}
	if (inp->SH) {
		sender->sender->cur = sender->sender->inAir;
		sender->sender->cur->Enter();
		return 0;
	}
	inp->animation.colour     = 3;
	inp->animation.normal     = 4;
	inp->animation.framecount = 11;
	inp->animation.fps        = 7.0;
	inp->speed = 1.0;
	inp->jump = vec2(0.7f, 1.5f);
	inp->visible = 0.5f;
	inp->camArmTarget = vec3(1.37f, 0.25f, 0.95) * inp->obj.sca.y;
	return 0;

}
int CrawlState::update(float deltaTime) {
	// cout << "CrawlState" << endl;
	inp->camArmTarget = vec3(1.37f, 0.25f, 0.95) * inp->obj.sca.y;
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





InAirState::InAirState(InputObject* inp, MovableState* sender) {
	this->sender = sender;
	this->inp = inp;
	dive = new DiveState(inp, this);
	roll = new RollState(inp, this);
	fall = new FallingState(inp, this);
	cur = fall;
}
int InAirState::Enter() {
	// cout << "InAirState" << endl;

	inp->resistance = vec3(1,1,1);
	if (inp->SH && inp->SP) {
		cur = dive;
	}
	else
	{
		cur = fall;
	}
	cur->Enter();
	inp->visible = 2;
	return 0;
}
int InAirState::update(float deltaTime) {
	if (inp->Grounded && cur != roll) {
		cur = roll;
		cur->Enter();
		 return 0;
	}
	inp->vel.y -= 5.5f * deltaTime;
	cur->update(deltaTime);
	return 0;
}

DiveState::DiveState(InputObject* inp, InAirState* sender) {
	this->sender = sender;
	this->inp = inp;
}
int DiveState::Enter() {

	// cout << "DiveState" << endl;

	inp->animation.colour     = 21;
	inp->animation.normal     = 22;
	inp->animation.framecount = 8;
	inp->animation.fps        = 10.0;
	inp->animation.current = 0.0;
	inp->vel.y = inp->jump.x;

	vec2 mov = vec2((inp->inp.y * sin(inp->rot.y) - inp->inp.x * cos(inp->rot.y)), (inp->inp.y * cos(inp->rot.y) + inp->inp.x * sin(inp->rot.y)));
	inp->vel.x = normalize(mov).x * inp->jump.y;
	inp->vel.z = normalize(mov).y * inp->jump.y;
	return 0;
}
int DiveState::update(float deltaTime) {
	if (inp->animation.current <= 3.0f) {
		inp->animation.fps = 20.0f;
	}
	if (inp->animation.current >= 7.0f) {
		inp->animation.current = 3.0f;
	}
	return 0;
}


RollState::RollState(InputObject* inp, InAirState* sender) {
	this->sender = sender;
	this->inp = inp;
}
int RollState::Enter() {
	// cout << "RollState" << endl;
	inp->vel.y = -0.1f;
	if (length(inp->vel) < 0.5f)
	{
		sender->sender->cur = sender->sender->idle;
		sender->sender->cur->Enter();
		return 0;
	}
	else
	{

		inp->animation.colour     = 19;
		inp->animation.normal     = 20;
		inp->animation.framecount = 6;
		inp->animation.fps        = 10.0;
		inp->animation.current = 0.0;

	}

	return 0;
}
int RollState::update(float deltaTime) {
	inp->vel.y = -1.1f;
	vec2 mov = vec2((inp->inp.y * sin(inp->rot.y) - inp->inp.x * cos(inp->rot.y)), (inp->inp.y * cos(inp->rot.y) + inp->inp.x * sin(inp->rot.y)));

	if (dot(mov, vec2(inp->vel.x, inp->vel.z)) > 0.5f)
	{
		inp->resistance.x = 1.0f;
		inp->resistance.z = 1.0f;
	}
	else
	{
		inp->resistance.x = 7.0f;
		inp->resistance.z = 7.0f;
	}
	if (inp->animation.current >= 5.0) {

		sender->sender->cur = sender->sender->idle;
		sender->sender->cur->Enter();
		return 0;
	}
	return 0;
}




FallingState::FallingState(InputObject* inp, InAirState* sender) {
	this->sender = sender;
	this->inp = inp;
}
int FallingState::Enter() {

	// cout << "FallingState" << endl;

	return 0;
}
int FallingState::update(float deltaTime) {
	
	return 0;
}

