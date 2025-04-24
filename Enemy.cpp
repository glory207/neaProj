#include "Enemy.h"
PathFind::PathFind(){}
void PathFind::FindPath(vec3 startpos, vec3 endpos, queue<PathFind*>* pathfq) {
	if ((!WaitingForPath && !OnPath) || (OnPath && EndPath)) {

	Startpos = startpos;
	Endpos = endpos;

	OnPath = false;
	EndPath = false;
	WaitingForPath = true;
	pathfq->push(this);
	}
}
Enemy::Enemy() {}
Enemy::Enemy(PlayerClass* pla, vec3 pos, vec3 rot) {
	inp = new EnemyInputObject();
	inp->player = pla;
	inp->pos = pos;
	inp->rot = rot;
	inp->obj = SpObj(pos, rot, vec3(0.15), initSpriteBuffer(), 28, 29);

	random_device rd;  // Seed generator
	mt19937 gen(rd()); // Mersenne Twister engine
	uniform_real_distribution<float> Rand(0.0f, 1.0f); // Range [0, 1]


	inp->animation = AnimationClass(35,36,10, Rand(gen) * 10,2);

	inp->vision = Light(pos + vec3(0,0.2,0));
	inp->vision.obj.sca = vec3(0.035);
	inp->vision.obj.text1 = texture(37);
	inp->vision.col = vec3(1, 0.3, 0.0);
	cur = new EnemyState(inp);
	cur->Enter();
}
void Enemy::update(float deltaTime,vec3 cam,Maze* mz, queue<PathFind*>* pathfq) {

	vec2 tmpL(inp->vel.x, inp->vel.z);
	// validates the directionso it does not
	// face down
	if (length(tmpL) != 0) {
		// faces the  vision cone to where the ememy moves
		tmpL = normalize(tmpL);
		// tilts the  vision cone down
		inp->lookTarget = vec3(tmpL.x, 0.5f,tmpL.y);
	}

	inp->look = inp->look + (inp->lookTarget - inp->look) * deltaTime * 5.0f;


	// when the end of the path is reached reset the path finder
	if (inp->pathFinder.pathP + 1 >= inp->pathFinder.path.size()) {
		inp->pathFinder.OnPath = false;
		inp->pathFinder.pathP = 0;
	}
	cur->update(deltaTime,mz,pathfq);

	inp->vel.z -= inp->vel.z * 7.0 * deltaTime;
	inp->vel.y -= inp->vel.y * 7.0 * deltaTime;
	inp->vel.x -= inp->vel.x * 7.0 * deltaTime;
	
	inp->pos.z -= inp->vel.z * deltaTime;
	inp->pos.y += inp->vel.y * deltaTime;
	inp->pos.x -= inp->vel.x * deltaTime;
	
	inp->rot.y = atan2(inp->pos.x - cam.x, inp->pos.z - cam.z);
	inp->obj.pos = vec3(inp->pos.x, inp->pos.y + inp->obj.sca.y, inp->pos.z);

	inp->animation.current += deltaTime * 10;
	
	// places the vision cone where the head should be
	inp->vision.pos = inp->obj.pos + vec3(-inp->lookTarget.x * 0.4f,0.5, -inp->lookTarget.z*0.4f) * inp->obj.sca.y;
	inp->vision.obj.rot.y = atan2(inp->look.x, inp->look.z);
	int dirr = floor(((
		atan2(inp->pos.x - cam.x, -inp->pos.z + cam.z)
		+ atan2(-inp->lookTarget.x, -inp->lookTarget.z)
		+ 0.125f * 3.141592f) / (0.25f * 3.141592f)));
	if (dirr != NULL) inp->animation.dir = dirr;
	else inp->animation.dir = 0;


	inp->obj.rot.y = inp->rot.y;
}

void Enemy::draw(float time, int shader) {

	inp->obj.text1 = texture(inp->animation.colour);
	inp->obj.text2 = texture(inp->animation.normal);
	inp->obj.textOff = inp->animation.sprite();
	inp->obj.textOff2 = inp->animation.sprite();
	inp->obj.draw(shader);
}
EnemyInputObject::EnemyInputObject() {}




EnemyState::EnemyState(EnemyInputObject* inp) {
	this->inp = inp;
	this->patrol = new PatrollingStae(inp, this);
	this->chase = new ChasingState(inp, this);
	this->cur = patrol;
}
int EnemyState::Enter() {
	cur->Enter();
	return 0;
}
int EnemyState::update(float deltaTime, Maze* mz, queue<PathFind*>* pathfq) {
	
	
	cur->update(deltaTime, mz, pathfq); return 0; }




PatrollingStae::PatrollingStae(EnemyInputObject* inp, EnemyState* sender) {
	this->inp = inp;
	this->sender = sender;
	this->idle = new EidleState(inp, this);
	this->walk = new EwalkingState(inp, this);
	this->cur = idle;
}
int PatrollingStae::Enter() {
	cur->Enter();
	return 0;
}
int PatrollingStae::update(float deltaTime, Maze* mz, queue<PathFind*>* pathfq) {
	if (inp->focus > 4) {
		sender->cur = sender->chase;
		sender->cur->Enter();
		inp->pathFinder.OnPath = false;
		return 0;
	}
	cur->update(deltaTime, mz, pathfq); return 0; }




EidleState::EidleState(EnemyInputObject* inp, PatrollingStae* sender) {
	this->inp = inp;
	this->sender = sender;
	
}
int EidleState::Enter() {

	timer = 3;
	inp->animation.colour = 31;
	inp->animation.normal = 32;
	inp->animation.framecount = 10;
	inp->animation.fps = 2.0; return 0;
}
int EidleState::update(float deltaTime, Maze* mz, queue<PathFind*>* pathfq) {
	timer -= deltaTime;
	if (timer < 0) {

		random_device rd;  // Seed generator
		mt19937 gen(rd()); // Mersenne Twister engine
		uniform_real_distribution<float> Rand(0.0f, 1.0f); // Range [0, 1]
		// gets a new path
		inp->pathFinder.FindPath(inp->pos, vec3(Rand(gen) * mz->count * mz->size, 0, Rand(gen) * mz->count * mz->size), pathfq);
		if (!inp->pathFinder.WaitingForPath)
		{
			sender->cur = sender->walk;
			sender->cur->Enter();
		}
	}
	return 0;
}




EwalkingState::EwalkingState(EnemyInputObject* inp, PatrollingStae* sender) {
	this->inp = inp;
	this->sender = sender;
}
int EwalkingState::Enter() {
	inp->animation.colour = 35;
	inp->animation.normal = 36;
	inp->animation.framecount = 10;
	inp->animation.fps = 2.0;

	return 0;
}
int EwalkingState::update(float deltaTime, Maze* mz, queue<PathFind*>* pathfq) {

	if (inp->pathFinder.OnPath) {
		// the cell grid the enemy is on
		vec2 psss1 = inp->pathFinder.path[int(inp->pathFinder.pathP)]->pos * mz->size;
		// the cell grid the player is moving towards
		vec2 psss2 = inp->pathFinder.path[int(inp->pathFinder.pathP + 1)]->pos * mz->size;
		// velelerate towardes the next point
		inp->vel += 4.5f * deltaTime * ((inp->pos - vec3(psss2.x, inp->pos.y, psss2.y)) / distance(psss2, vec2(inp->pos.x, inp->pos.z)));
		// set the players hight to the cell grid hight
		if (inp->pathFinder.path[inp->pathFinder.pathP + 1]->obstruction > 3 && inp->pos.y < inp->pathFinder.path[inp->pathFinder.pathP + 1]->hight * mz->size) {
			inp->pos.y += 0.5f * deltaTime;
			if (inp->pos.y > inp->pathFinder.path[inp->pathFinder.pathP + 1]->hight * mz->size)inp->pos.y = inp->pathFinder.path[inp->pathFinder.pathP + 1]->hight * mz->size;
		}
		else
		{
			inp->pos.y -= 1.0f * deltaTime;
			if (inp->pos.y < 0)inp->pos.y = 0;
		}
		// when closer to the next point than the current point make it the new current point
		if (distance(psss1, vec2(inp->pos.x, inp->pos.z)) > distance(psss2, vec2(inp->pos.x, inp->pos.z)))inp->pathFinder.pathP++;

	}
	else{
		sender->cur = sender->idle;
		sender->cur->Enter();
	}
	return 0;
}




ChasingState::ChasingState(EnemyInputObject* inp, EnemyState* sender) {
	this->inp = inp;
	this->sender = sender;
	this->follow = new followingState(inp, this);
	this->drain = new drainingState(inp, this);
	this->cur = follow;
}
int ChasingState::Enter() { this->cur->Enter(); return 0; }
int ChasingState::update(float deltaTime, Maze* mz, queue<PathFind*>* pathfq) { cur->update(deltaTime, mz, pathfq); return 0; }


followingState::followingState(EnemyInputObject* inp, ChasingState* sender) {
	this->inp = inp;
	this->sender = sender;
}
int followingState::Enter() {
	inp->animation.colour = 33;
	inp->animation.normal = 34;
	inp->animation.framecount = 10;
	inp->animation.fps = 2.0;
	timer = 0;
	return 0;
}
int followingState::update(float deltaTime, Maze* mz, queue<PathFind*>* pathfq) {
	timer -= deltaTime;
	if (inp->pathFinder.OnPath && timer > 0) {
		// the cell grid the enemy is on
		vec2 psss1 = inp->pathFinder.path[int(inp->pathFinder.pathP)]->pos * mz->size;
		// the cell grid the player is moving towards
		vec2 psss2 = inp->pathFinder.path[int(inp->pathFinder.pathP + 1)]->pos * mz->size;
		// velelerate towardes the next point
		inp->vel += 11.0f * deltaTime * ((inp->pos - vec3(psss2.x, inp->pos.y, psss2.y)) / distance(psss2, vec2(inp->pos.x, inp->pos.z)));
		// set the players hight to the cell grid hight
		if (inp->pathFinder.path[inp->pathFinder.pathP + 1]->obstruction > 3 && inp->pos.y < inp->pathFinder.path[inp->pathFinder.pathP + 1]->hight * mz->size) {
			inp->pos.y += 0.5f * deltaTime;
			if (inp->pos.y > inp->pathFinder.path[inp->pathFinder.pathP + 1]->hight * mz->size)inp->pos.y = inp->pathFinder.path[inp->pathFinder.pathP + 1]->hight * mz->size;
		}
		else
		{
			inp->pos.y -= 1.0f * deltaTime;
			if (inp->pos.y < 0)inp->pos.y = 0;
		}
		// when closer to the next point than the current point make it the new current point
		if (distance(psss1, vec2(inp->pos.x, inp->pos.z)) > distance(psss2, vec2(inp->pos.x, inp->pos.z)))inp->pathFinder.pathP++;

	}
	else {
		timer = 1;
		inp->pathFinder.OnPath = false;
		inp->pathFinder.FindPath(inp->pos, inp->player->inp->pos, pathfq);
	}
	// stops multiple enemies from grabbing the player
	if (inp->player->inp->grabed || inp->focus < 2) {

		inp->focus = 0;
		inp->pathFinder.OnPath = false;
		sender->sender->cur = sender->sender->patrol;
		sender->sender->cur->Enter();

		return 0;
	}
	// when they are close enough to grab
	if (distance(vec2(inp->pos.x, inp->pos.z), vec2(inp->player->inp->pos.x, inp->player->inp->pos.z)) < 0.2) {
		inp->player->inp->grabed = true;
		sender->cur = sender->drain;
		sender->Enter();
	}
	
	return 0; }




drainingState::drainingState(EnemyInputObject* inp, ChasingState* sender) {
	this->inp = inp;
	this->sender = sender;
}
int drainingState::Enter() {

	inp->animation.colour = 29;
	inp->animation.normal = 30;
	inp->animation.framecount = 10;
	inp->animation.fps = 2.0;

	timer = 10; 
	return 0;
}
int drainingState::update(float deltaTime, Maze* mz, queue<PathFind*>* pathfq) {
	timer -= deltaTime;
	inp->player->inp->grabed = true;
	// makes the enemy hold the player
	inp->player->inp->pos = vec3(inp->vision.pos.x, inp->obj.sca.y , inp->vision.pos.z)
		+ vec3(inp->lookTarget.x, 0, inp->lookTarget.z) * -0.03f;
	inp->player->inp->vel = vec3(0);

	if (inp->pathFinder.OnPath) {
		// the cell grid the enemy is on
		vec2 psss1 = inp->pathFinder.path[int(inp->pathFinder.pathP)]->pos * mz->size;
		// the cell grid the player is moving towards
		vec2 psss2 = inp->pathFinder.path[int(inp->pathFinder.pathP + 1)]->pos * mz->size;
		// velelerate towardes the next point
		inp->vel += 9.0f * deltaTime * ((inp->pos - vec3(psss2.x, inp->pos.y, psss2.y)) / distance(psss2, vec2(inp->pos.x, inp->pos.z)));
		// set the players hight to the cell grid hight
		if (inp->pathFinder.path[inp->pathFinder.pathP + 1]->obstruction > 3 && inp->pos.y < inp->pathFinder.path[inp->pathFinder.pathP + 1]->hight * mz->size) {
			inp->pos.y += 0.5f * deltaTime;
			if (inp->pos.y > inp->pathFinder.path[inp->pathFinder.pathP + 1]->hight * mz->size)inp->pos.y = inp->pathFinder.path[inp->pathFinder.pathP + 1]->hight * mz->size;
		}
		else
		{
			inp->pos.y -= 1.0f * deltaTime;
			if (inp->pos.y < 0)inp->pos.y = 0;
		}
		// when closer to the next point than the current point make it the new current point
		if (distance(psss1, vec2(inp->pos.x, inp->pos.z)) > distance(psss2, vec2(inp->pos.x, inp->pos.z)))inp->pathFinder.pathP++;

	}
	else {
		random_device rd;  // Seed generator
		mt19937 gen(rd()); // Mersenne Twister engine
		uniform_real_distribution<float> Rand(0.0f, 1.0f); // Range [0, 1]
		// gets a new path
		inp->pathFinder.FindPath(inp->pos, vec3(Rand(gen) * mz->count * mz->size, 0, Rand(gen) * mz->count * mz->size), pathfq);
	}
	if (timer < 0) {
		// after 10 secconds the player is released
		inp->player->inp->vel = vec3(inp->lookTarget.x,0, inp->lookTarget.z) * 3.03f;
		inp->focus = 0;
		// the enemy is teleported away
		random_device rd;  // Seed generator
		mt19937 gen(rd()); // Mersenne Twister engine
		uniform_real_distribution<float> Rand(0.0f, 1.0f); // Range [0, 1]
		inp->pos = vec3(Rand(gen) * mz->count * mz->size, 0, Rand(gen) * mz->count * mz->size);
		inp->pathFinder.OnPath = false;
		inp->player->inp->grabed = false;
		sender->sender->cur = sender->sender->patrol;
		sender->sender->cur->Enter();
	}
	return 0; }