
#include "PlayerClass.h"
#include "MovableStateClass.h"


PlayerClass::PlayerClass(){}
PlayerClass::PlayerClass(vec3 pos, vec3 rot) {
	inp = new InputObject(pos, rot);

	obj = SpObj(pos, rot, vec3(0.05), initSpriteBuffer(), 6, 9);


	movable = new MovableState(inp, nullptr);
	cur = movable;

	cur->Enter();
}
void PlayerClass::update(float deltaTime) {
	inp->animation.dir = 0;


	obj.pos = vec3(inp->pos.x, inp->pos.y + obj.sca.y, inp->pos.z);
	inp->animation.current += deltaTime * inp->animation.fps;
	
	cur->update(deltaTime);

	obj.rot.y = inp->rot.y;
}
void PlayerClass::draw(float time, int shader) {

	obj.text1 = texture(inp->animation.colour);
	obj.text2 = texture(inp->animation.normal);
	obj.textOff = inp->animation.sprite();
	obj.textOff2 = inp->animation.sprite();
	obj.draw(shader);

}
AnimationClass::AnimationClass(int col, int nor, int cnt, float cur, float f) {
	colour = col;
	normal = nor;
	framecount = cnt;
	fps = f;
	current = cur;
}
AnimationClass::AnimationClass() {
	colour = 0;
	normal = 0;
	framecount = 1;
	fps = 1;
	current = 0;
}
vec4 AnimationClass::sprite() {
	return vec4(
		(int)(current) % (framecount - 1),
		dir % 8,
		1.0 / framecount,
		1.0 / 8.0);
}












PlayerClassOld::PlayerClassOld() {}
PlayerClassOld::PlayerClassOld(vec3 pos, vec3 rot) {
	inp = new InputObject(pos, rot);

	obj = SpObj(pos, rot, vec3(0.05), initSpriteBuffer(), 6, 9);


	inp->animation.colour = 1;
	inp->animation.normal = 6;
	inp->animation.framecount = 10;
	inp->animation.fps = 8.0;
}
void PlayerClassOld::update(float deltaTime) {
	inp->animation.dir = 0;


	// rotates the direction of the input so it corresponds with the direction to the player
	vec3 move = vec3(inp->inp.x * cos(inp->rot.y) - inp->inp.y * sin(inp->rot.y),
		0, -inp->inp.y * cos(inp->rot.y) - inp->inp.x * sin(inp->rot.y));
	// stops the corner strafing from being faster
	if (length(move) > 1)move = normalize(move);
	// updates the velocity which in turn updates the position
	inp->vel += move * 5.0f * deltaTime;
	// reduces the velocity by a resistance factor
	inp->vel -= inp->vel * 7.0f * deltaTime;
	inp->pos += inp->vel * deltaTime;

  	int dir = floor(((
  		-atan2(inp->pos.x - inp->cam.x, -inp->pos.z + inp->cam.z)
  		+ atan2(-inp->vel.x, inp->vel.z)
  		+ 0.125 * 3.141592) / (0.25 * 3.141592)));
  	if (dir != NULL) inp->animation.dir = dir;

	obj.pos = vec3(inp->pos.x, inp->pos.y + obj.sca.y, inp->pos.z);
	inp->animation.current += deltaTime * inp->animation.fps;


	obj.rot.y = inp->rot.y;
}
void PlayerClassOld::draw(float time, int shader) {

	obj.text1 = texture(inp->animation.colour);
	obj.text2 = texture(inp->animation.normal);
	obj.textOff = inp->animation.sprite();
	obj.textOff2 = inp->animation.sprite();
	obj.draw(shader);

}