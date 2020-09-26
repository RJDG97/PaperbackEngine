#include "LightingSystem.h"
#include "GraphicsSystem.h"
#include "Core.h"
#include "WindowsSystem.h"
#include "ModelManager.h"

LightingSystem::PointLight::PointLight(GLint x, GLint y,
									   GLfloat red, GLfloat green, GLfloat blue,
									   GLfloat radius,
									   GLfloat intensity)
	: x_{x}, y_{y},
	  red_{ red }, green_{ green }, blue_{blue},
	  radius_{radius},
	  intensity_{intensity}
{

}

void LightingSystem::Init()
{
	windows_system = CORE->GetSystem<WindowsSystem>("WindowsSystem");

	GLint width = windows_system->getWinWidth();
	GLint height = windows_system->getWinHeight();

	//MODELMANAGER->AddTristripsModel(1, 1, "Shaders/lighting.vert", "Shaders/lighting.frag", ModelType::LightModel);

	glGenFramebuffers(1, &frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	
	glGenTextures(1, &final_texture);
	glBindTexture(GL_TEXTURE_2D, final_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, final_texture, 0);

	glGenRenderbuffers(1, &render_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, render_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, render_buffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer is not complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenFramebuffers(1, &lighting_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, lighting_buffer);

	glGenTextures(1, &lighting_texture);
	glBindTexture(GL_TEXTURE_2D, lighting_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width/2, height/2, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lighting_texture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Lighting buffer is not complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightingSystem::Update(float frametime)
{

}

void LightingSystem::TestDraw()
{
	/*
	//reset the lighting texture
	glBindFramebuffer(GL_FRAMEBUFFER, lighting_buffer);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.1f, 0.05f, 0.2f, 1.0f);

	//render the lights here
	glBlendFunc(GL_ONE, GL_ONE);
	glViewport(400/2 - 150, 300/2 - 150, 150 * 2, 150 * 2); //Use this equation BUT need multiply stuff by intensity!!
	CORE->GetSystem<GraphicsSystem>("GraphicsSystem")->models[1].DrawLight(glm::vec3{ 1.0f, 1.0f, 0.5f },
												   glm::vec2{ 400/2, 300/2},
							  					   1.0f, 150.0f);
	
	glViewport(200 / 2 - 150, 100 / 2 - 150, 150 * 2, 150 * 2);
	CORE->GetSystem<GraphicsSystem>("GraphicsSystem")->models[1].DrawLight(glm::vec3{ 0.3f, 0.9f, 1.0f },
		glm::vec2{ 200/2, 100/2 },
		1.0f, 150.0f);

	glViewport(600 / 2 - 150, 100 / 2 - 150, 150 * 2, 150 * 2);
	CORE->GetSystem<GraphicsSystem>("GraphicsSystem")->models[1].DrawLight(glm::vec3{ 1.0f, 0.3f, 1.0f },
		glm::vec2{ 600/2, 100/2 },
		1.0f, 150.0f);*/
}

void LightingSystem::Cleanup()
{
	glDeleteFramebuffers(1, &frame_buffer);
	glDeleteRenderbuffers(1, &render_buffer);
	glDeleteTextures(1, &final_texture);
}

GLuint& LightingSystem::GetFrameBuffer()
{
	return frame_buffer;
}

GLuint& LightingSystem::GetFinalTexture()
{
	return final_texture;
}

GLuint& LightingSystem::GetLightingTexture()
{
	return lighting_texture;
}

std::string LightingSystem::GetName()
{
	return "LightingSystem";
}

void LightingSystem::SendMessageD(Message* m)
{
}
