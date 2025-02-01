
#include "PlayerClass.h"
#include "MovableStateClass.h"


PlayerClass::PlayerClass(){}
PlayerClass::PlayerClass(vec3 pos, vec3 rot) {
	inp = new InputObject(pos, rot);

	obj = SpObj(pos, rot, vec3(0.1), initCubeBuffer({ 9}), 6, 9);


	movable = new MovableState(inp, nullptr);
	cur = movable;

	cur->Enter();
}
void PlayerClass::update(float deltaTime) {
	inp->dir.x = 0;


	obj.pos = vec3(inp->pos.x, inp->pos.y + obj.sca.y, inp->pos.z);
	inp->animation[4] += deltaTime * inp->animation[3];
	
	cur->update(deltaTime);

	obj.rot.y = inp->rot.y;
}
void PlayerClass::draw(float time, int shader) {



	obj.text1 = texture(inp->animation[0]);
	obj.text2 = texture(inp->animation[1]);
	obj.textOff = vec4(
		(int)(inp->animation[4]) % ((int)(inp->animation[2]) - 1),
		(int)(inp->dir.x) % 8,
		1.0 / inp->animation[2],
		1.0 / 8.0);
	obj.draw(shader);
}
