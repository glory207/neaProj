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
Enemy::Enemy(vec3 pos, vec3 rot) {
	this->pos = pos;
	this->rot = rot;
	obj = SpObj(pos, rot, vec3(0.15), initSpriteBuffer(), 28, 29);

	random_device rd;  // Seed generator
	mt19937 gen(rd()); // Mersenne Twister engine
	uniform_real_distribution<float> Rand(0.0f, 1.0f); // Range [0, 1]

	animation = AnimationClass(35,36,10, Rand(gen) * 10,2);

	vision = Light(pos + vec3(0,0.2,0));
	vision.obj.sca = vec3(0.035);
	vision.obj.text1 = texture(37);
	vision.col = vec3(1, 0.3, 0.0);
}
void Enemy::update(float deltaTime,vec3 cam,Maze* mz, queue<PathFind*>* pathfq) {

	vec2 tmpL(vel.x, vel.z);
	// validates the directionso it does not
	// face down
	if (length(tmpL) > 0.01f) {
		// faces the  vision cone to where the ememy moves
		tmpL = normalize(tmpL);
		// tilts the  vision cone down
		lookTarget = vec3(tmpL.x, 0.5f,tmpL.y);
	}

	look = look + (lookTarget - look) * deltaTime * 5.0f;

	if (!pathFinder.WaitingForPath) {
		random_device rd;  // Seed generator
		mt19937 gen(rd()); // Mersenne Twister engine
		uniform_real_distribution<float> Rand(0.0f, 1.0f); // Range [0, 1]
		// gets a new path
		pathFinder.FindPath(pos,vec3(Rand(gen) * mz->count * mz->size,0, Rand(gen) * mz->count * mz->size), pathfq); 
	}

	// when the end of the path is reached reset the path finder
	if (pathFinder.pathP + 1 >= pathFinder.path.size()) {
		pathFinder.OnPath = false;
		pathFinder.pathP = 0;
	}
	if (pathFinder.OnPath) {
		// the cell grid the enemy is on
		vec2 psss1 = pathFinder.path[int(pathFinder.pathP)]->pos * mz->size;
		// the cell grid the player is moving towards
		vec2 psss2 = pathFinder.path[int(pathFinder.pathP + 1)]->pos * mz->size;
		// velelerate towardes the next point
		vel += 1.5f * deltaTime * ((pos - vec3(psss2.x, pos.y, psss2.y)) / distance(psss2, vec2(pos.x, pos.z)));
		// set the players hight to the cell grid hight
		if (pathFinder.path[pathFinder.pathP + 1]->obstruction > 3 && pos.y < pathFinder.path[pathFinder.pathP + 1]->hight * mz->size) {
			pos.y += 1.0f * deltaTime;
			if (pos.y > pathFinder.path[pathFinder.pathP + 1]->hight * mz->size)pos.y = pathFinder.path[pathFinder.pathP + 1]->hight * mz->size;
		}
		else
		{
			pos.y -= 1.0f * deltaTime;
			if (pos.y < 0)pos.y = 0;
		}
		// when closer to the next point than the current point make it the new current point
		if (distance(psss1, vec2(pos.x, pos.z)) > distance(psss2, vec2(pos.x, pos.z)))pathFinder.pathP++;

	}

	vel.z -= vel.z * 7.0 * deltaTime;
	vel.y -= vel.y * 7.0 * deltaTime;
	vel.x -= vel.x * 7.0 * deltaTime;
	
	pos.z -= vel.z * deltaTime;
	pos.y += vel.y * deltaTime;
	pos.x -= vel.x * deltaTime;
	
	rot.y = atan2(pos.x - cam.x, pos.z - cam.z);
	obj.pos = vec3(pos.x, pos.y + obj.sca.y, pos.z);

	animation.current += deltaTime * 10;
	
	// places the vision cone where the head should be
	vision.pos = obj.pos + vec3(-lookTarget.x * 0.4f,0.5, -lookTarget.z*0.4f) *  obj.sca.y;
	vision.obj.rot.y = atan2(look.x, look.z);
	int dirr = floor(((
		atan2(pos.x - cam.x, -pos.z + cam.z)
		+ atan2(-vel.x, -vel.z)
		+ 0.125f * 3.141592f) / (0.25f * 3.141592f)));
	if (dirr != NULL) animation.dir = dirr;
	else animation.dir = 0;


	obj.rot.y = rot.y;
}

void Enemy::draw(float time, int shader) {

	obj.text1 = texture(animation.colour);
	obj.text2 = texture(animation.normal);
	obj.textOff = animation.sprite();
	obj.textOff2 = animation.sprite();
	obj.draw(shader);
}