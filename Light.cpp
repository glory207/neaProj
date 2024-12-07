#include "Light.h"
#include <glad/glad.h>

Light::Light(glm::vec3 pos) {
	this->pos = pos;
	obj = SpObj(pos, rot, glm::vec3(0.02f), initCubeBuffer({0,1,2,3,4,5}),0,0);
	col = glm::vec3(0.1f);
	
	glGenTextures(1, &depthTex);

	glBindTexture(GL_TEXTURE_CUBE_MAP, depthTex);
	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,GL_DEPTH_COMPONENT,500,500,0, GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	handle = glGetTextureHandleARB(depthTex);
	glMakeTextureHandleResidentARB(handle);
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
}


