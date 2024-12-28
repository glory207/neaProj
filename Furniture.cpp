#include "Furniture.h"
#include <random>
Furniture::Furniture(vec3 ps, vec3 sc, float rt) {
	std::random_device rd;  // Seed generator
	std::mt19937 gen(rd()); // Mersenne Twister engine
	std::uniform_real_distribution<float> Rand(0.0f, 1.0f); // Range [0, 1]

	pos = ps;
	sca = sc;
	rot = vec3(0.0f, rt, 0.0f);

}
Furniture::Furniture() {
}