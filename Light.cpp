#include "Light.h"
#include <glad/glad.h>
#include <random>

Light::Light(glm::vec3 pos) {
	size = 500;
	this->pos = pos;
	obj = SpObj(pos, rot, glm::vec3(0.09f), initCubeBuffer({9}),27,0);
	col = glm::vec3(0.1f);
	obj.pos = pos;
	obj.rot = rot;
	rotationMatrix = glm::mat4();
	rotationMatrix = glm::rotate(rotationMatrix, -rot.x, glm::vec3(1, 0, 0));
	rotationMatrix = glm::rotate(rotationMatrix, -rot.y, glm::vec3(0, 1, 0));
	rotationMatrix = glm::rotate(rotationMatrix, -rot.z, glm::vec3(0, 0, 1));
	active = false;
}
void Light::activate(bool act) {
	
	if(!active && act)
	{

		glGenTextures(1, &depthTex);

		glBindTexture(GL_TEXTURE_CUBE_MAP, depthTex);
		for (int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		
		active = true;
	}
	else if (active && !act)
	{
		
		glDeleteTextures(1, &depthTex);
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


