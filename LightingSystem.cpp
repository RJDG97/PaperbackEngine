#include "LightingSystem.h"
#include "glhelper.h"

void LightingSystem::Init()
{
	glGenFramebuffers(1, &frame_buffer);
	glGenTextures(1, &final_texture);
	glBindTexture(GL_TEXTURE_2D, final_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GLHelper::Instance()->width, GLHelper::Instance()->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glGenRenderbuffers(1, &render_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, render_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16,
		GLHelper::Instance()->width, GLHelper::Instance()->height);

	//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, final_texture, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, final_texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, render_buffer);
	//GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	//glDrawBuffers(1, DrawBuffers);
}

void LightingSystem::Update()
{

}

void LightingSystem::Cleanup()
{

}

GLuint LightingSystem::GetFrameBuffer()
{
	return frame_buffer;
}

GLuint LightingSystem::GetRenderBuffer()
{
	return render_buffer;
}

GLuint LightingSystem::GetFinalTexture()
{
	return final_texture;
}
