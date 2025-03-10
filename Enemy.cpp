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
vec3 PathFind::startpos() { return Startpos; }
vec3 PathFind::endpos() { return Endpos; }
Enemy::Enemy() {}
Enemy::Enemy(vec3 pos, vec3 rot) {
	this->pos = pos;
	this->rot = rot;
	obj = SpObj(pos, rot, vec3(0.2), initCubeBuffer({ 9 }), 28,29);

	random_device rd;  // Seed generator
	mt19937 gen(rd()); // Mersenne Twister engine
	uniform_real_distribution<float> Rand(0.0f, 1.0f); // Range [0, 1]
	frame = Rand(gen) * 15;
}
void Enemy::update(float deltaTime,vec3 cam,Maze* mz, queue<PathFind*>* pathfq) {

	 //Define the random number generator and distribution
	if (!pathFinder.WaitingForPath) {
		random_device rd;  // Seed generator
		mt19937 gen(rd()); // Mersenne Twister engine
		uniform_real_distribution<float> Rand(0.0f, 1.0f); // Range [0, 1]
		pathFinder.FindPath(vec3(int(9.0 * (pos.x / mz->size + mz->thk) / (mz->thk * 2.0)) - 10 * int((pos.x + 1.0f) / mz->size),
			int(9.0 * (pos.z / mz->size + mz->thk) / (mz->thk * 2.0)) - 10 * int((pos.z + 1.0f) / mz->size),
			int((pos.x + 1.0f) / mz->size) + int((pos.z + 1.0f) / mz->size) * mz->count),
			vec3(5, 5, int(Rand(gen) * mz->count * mz->count)), pathfq);
	}
	if (pathFinder.pathP + 1 >= pathFinder.path.size()) {
		pathFinder.OnPath = false;
		pathFinder.pathP = 0;
	}
	if (pathFinder.OnPath) {
		vec2 psss1 = pathFinder.path[int(pathFinder.pathP)]->pos * mz->size;
		vec2 psss2 = pathFinder.path[int(pathFinder.pathP + 1)]->pos * mz->size;
	
		acc += deltaTime * 3.0f * ((pos - vec3(psss2.x, pos.y, psss2.y)) / distance(psss2, vec2(pos.x, pos.z)));
		if (distance(psss1, vec2(pos.x, pos.z)) > distance(psss2, vec2(pos.x, pos.z)))
			pathFinder.pathP++;
	
	}
	acc.z -= acc.z * 7.0 * deltaTime;
	acc.y -= acc.y * 7.0 * deltaTime;
	acc.x -= acc.x * 7.0 * deltaTime;
	
	pos.z -= acc.z * deltaTime;
	pos.y += acc.y * deltaTime;
	pos.x -= acc.x * deltaTime;
	
	rot.y = int(atan2(pos.x - cam.x, pos.z - cam.z)/ (3.141592f /8.0)) * (3.141592f / 8.0);
	rot.y = atan2(pos.x - cam.x, pos.z - cam.z);
	obj.pos = vec3(pos.x, pos.y + obj.sca.y, pos.z);
	frame += deltaTime * 10;
	
	
	int dirr = floor(((
		atan2(pos.x - cam.x, -pos.z + cam.z)
		+ atan2(-acc.x, -acc.z)
		+ 0.125f * 3.141592f) / (0.25f * 3.141592f)));
	if (dirr != NULL) dir = dirr;
	else dir = 0;
	obj.textOff = vec4(
		(int)(frame) % 14,
		(int)(dir) % 8,
		1.0 / 14.0,
		1.0 / 8.0);
	obj.textOff2 = vec4(
		(int)(frame) % 14,
		(int)(dir) % 8,
		1.0 / 14.0,
		1.0 / 8.0);
	obj.rot.y = rot.y;
}

void Enemy::draw(float time, int shader) {

	obj.draw(shader);
}