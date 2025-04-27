#include "animation.h"

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
		// the number of sprites
		(int)(current) % (framecount - 1),
		// the 8 directions
		dir % 8,
		1.0 / framecount,
		1.0 / 8.0);
}