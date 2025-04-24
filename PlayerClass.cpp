
#include "PlayerClass.h"
#include "MovableStateClass.h"


PlayerClass::PlayerClass(){}
PlayerClass::PlayerClass(vec3 pos, vec3 rot) {
	inp = new InputObject(pos, rot);

	inp->obj = SpObj(pos, rot, vec3(0.05), initSpriteBuffer(), 6, 9);


	movable = new MovableState(inp, nullptr);
	cur = movable;

	cur->Enter();
}
void PlayerClass::update(float deltaTime) {
	inp->animation.dir = 0;

	inp->obj.pos = vec3(inp->pos.x, inp->pos.y + inp->obj.sca.y, inp->pos.z);
	inp->animation.current += deltaTime * inp->animation.fps;
	
	cur->update(deltaTime);

	inp->obj.rot.y = inp->rot.y;
}
void PlayerClass::draw(float time, int shader) {

	inp->obj.text1 = texture(inp->animation.colour);
	inp->obj.text2 = texture(inp->animation.normal);
	inp->obj.textOff = inp->animation.sprite();
	inp->obj.textOff2 = inp->animation.sprite();
	inp->obj.draw(shader);

}


