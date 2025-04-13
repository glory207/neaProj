#include "Light.h"
#include <glad/glad.h>
#include <random>

Light::Light(glm::vec3 pos) {
	std::random_device rd;  // Seed generator
	std::mt19937 gen(rd()); // Mersenne Twister engine
	std::uniform_real_distribution<float> Rand(0.0f, 1.0f); // Range [0, 1]

	size = 500;
	this->pos = pos;
	obj = SpObj(pos, rot, glm::vec3(0.09f), initSpriteBuffer(),27,0);
	col = glm::vec3(1,0.5,0.2);
	obj.pos = pos;
	obj.rot = rot;
	rotationMatrix = glm::mat4();
	rotationMatrix = glm::rotate(rotationMatrix, -rot.x, glm::vec3(1, 0, 0));
	rotationMatrix = glm::rotate(rotationMatrix, -rot.y, glm::vec3(0, 1, 0));
	rotationMatrix = glm::rotate(rotationMatrix, -rot.z, glm::vec3(0, 0, 1));
	active = false;


}																																				 
void Light::activate(bool act) {	

	firstActive = false;
	if(!active && act)
	{

		// creates 2 textures that folds into cubes so they can record data from every angle
		// the first one will be used for stationary objects and only be updated once																																			 
		glGenTextures(1, &depthTexPre);
		// makes it the texture being edited
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthTexPre);
		for (int i = 0; i < 6; i++)
		{
			// gets each of the 6 faces and sets its resolution and format
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, size, size,
				0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}
		// the upscaling and downscaling are pixelated 
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// if the image is sampled out of its bounds the last valid value is used
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		// the second texture for moving objects that update every frame
		glGenTextures(1, &depthTex);
		// makes it the texture being edited
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthTex);

		for (int i = 0; i < 6; i++)
		{
			// gets each of the 6 faces and sets its resolution and format
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, size, size,
				0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}
		// the upscaling and downscaling are pixelated 
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// if the image is sampled out of its bounds the last valid value is used
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		// stops firstActive from being true next time as active is true 
		firstActive = true;
		active = true;
	}
	else if (active && !act)
	{
		// if nolonger needed the textures are deleted to save memory
		glDeleteTextures(1, &depthTex);
		glDeleteTextures(1, &depthTexPre);
		// the next time its used new textures should be generated
		active = false;
	}
	
	
	
}
Light::Light() {

}
void Light::update(float deltaTime) {


	//acc.y -= 2.1 * deltaTime;
	acc.x *= 0.985;
	acc.y *= 0.985;
	acc.z *= 0.985;

	pos.x += acc.x * deltaTime;
	pos.y += acc.y * deltaTime;
	pos.z += acc.z * deltaTime;
	obj.pos = pos;
	obj.rot = rot;
	rotationMatrix = glm::mat4();
	rotationMatrix = glm::rotate(rotationMatrix, -rot.x, glm::vec3(1, 0, 0));
	rotationMatrix = glm::rotate(rotationMatrix, -rot.y, glm::vec3(0, 1, 0));
	rotationMatrix = glm::rotate(rotationMatrix, -rot.z, glm::vec3(0, 0, 1));
}


