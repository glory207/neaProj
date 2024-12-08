#include "frameBuffer.h"
GframeBuffer::GframeBuffer(glm::vec2 sz) {
	size = sz;
	glGenTextures(1, &ColTex);
	glGenTextures(1, &PosTex);
	glGenTextures(1, &NomTex);
	glGenTextures(1, &NomFTex);
	glGenFramebuffers(1, &FB);
	GLuint RB;
	glGenRenderbuffers(1, &RB);

	//Col
	glBindTexture(GL_TEXTURE_2D, ColTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_2D);
	//pos
	glBindTexture(GL_TEXTURE_2D, PosTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_2D);
	//nom
	glBindTexture(GL_TEXTURE_2D, NomTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_2D);
	//nomF
	glBindTexture(GL_TEXTURE_2D, NomFTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindFramebuffer(GL_FRAMEBUFFER, FB);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColTex, 0);                  
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, PosTex, 0);                  
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, NomTex, 0);                  
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, NomFTex, 0);                 

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, drawBuffers);

	glBindRenderbuffer(GL_RENDERBUFFER, RB);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RB);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
GframeBuffer::GframeBuffer() {

}
void GframeBuffer::bind(bool clear) {
	glViewport(0, 0, size.x, size.y);

	glBindFramebuffer(GL_FRAMEBUFFER, FB);
	glClearColor(0.0, 0.0, 0.0, 0.0); // Clear to black, fully opaque
	glClearDepth(1.0); // Clear everything
	glEnable(GL_DEPTH_TEST); // Enable depth testing
	glEnable(GL_BLEND); // Enable depth testing
	glDepthFunc(GL_LEQUAL); // Near things obscure far things
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	// Clear the canvas before we start drawing on it.


	if (clear) glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}




createShadowFramebufferCube::createShadowFramebufferCube() {}
createShadowFramebufferCube::createShadowFramebufferCube(float sz) {

	GLuint ColTex;
	glGenTextures(1, &ColTex);
	glBindTexture(GL_TEXTURE_2D, ColTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 500, 500, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);




	glGenFramebuffers(1,&FB);
	glBindFramebuffer(GL_FRAMEBUFFER,FB);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	face = { 
		GL_TEXTURE_CUBE_MAP_POSITIVE_X, 
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 
	};
	target = {
		glm::vec3(1, 0, 0),
		glm::vec3(-1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, -1, 0),
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, -1)
	};
	up = {
		glm::vec3(0, -1, 0),
		glm::vec3(0, -1, 0),
		glm::vec3(0, 0, 1) ,
		glm::vec3(0, 0, -1),
		glm::vec3(0, -1, 0),
		glm::vec3(0, -1, 0)
	};


}
void createShadowFramebufferCube::bind(bool clear,int i, GLuint depthTex) {

	glBindTexture(GL_TEXTURE_CUBE_MAP, depthTex);
	glBindFramebuffer(GL_FRAMEBUFFER, FB);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTex, 0);
	glDrawBuffer(GL_NONE); 
	glReadBuffer(GL_NONE); 


	
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		switch (status) {
		case GL_FRAMEBUFFER_UNDEFINED:
			std::cout << "Framebuffer error: GL_FRAMEBUFFER_UNDEFINED (Target is not the default framebuffer)." << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			std::cout << "Framebuffer error: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT (Attachment points are incomplete or mismatched)." << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			std::cout << "Framebuffer error: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT (No images attached to the framebuffer)." << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			std::cout << "Framebuffer error: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER (Draw buffer is not specified or incomplete)." << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			std::cout << "Framebuffer error: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER (Read buffer is not specified or incomplete)." << std::endl;
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			std::cout << "Framebuffer error: GL_FRAMEBUFFER_UNSUPPORTED (Framebuffer format or configuration is not supported by the implementation)." << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			std::cout << "Framebuffer error: GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE (Number of samples for attachments mismatch)." << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			std::cout << "Framebuffer error: GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS (Layered attachments have inconsistent targets)." << std::endl;
			break;
		default:
			std::cout << "Framebuffer error: Unknown (Code: " << status << ")." << std::endl;
			break;
		}
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f); // Clear everything
	glEnable(GL_DEPTH_TEST); // Enable depth testing
	glDepthFunc(GL_LEQUAL); // Near things obscure far things

	if (clear) glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



FrameBuffer::FrameBuffer(glm::vec2 sz) {
	size = sz;
	glGenTextures(1, &ColTex);
	glGenFramebuffers(1, &FB);
	GLuint RB;
	glGenRenderbuffers(1, &RB);

	//Col
	glBindTexture(GL_TEXTURE_2D, ColTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, FB);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColTex, 0);


	glBindRenderbuffer(GL_RENDERBUFFER, RB);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RB);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
FrameBuffer::FrameBuffer() {

}
void FrameBuffer::bind(bool clear) {
	glViewport(0, 0, size.x, size.y);

	glBindFramebuffer(GL_FRAMEBUFFER, FB);
	glClearColor(0.0, 0.0, 0.0, 0.0); // Clear to black, fully opaque
	glClearDepth(1.0); // Clear everything
	glEnable(GL_DEPTH_TEST); // Enable depth testing
	glEnable(GL_BLEND); // Enable depth testing
	glDepthFunc(GL_LEQUAL); // Near things obscure far things
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	// Clear the canvas before we start drawing on it.


	if (clear) glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

