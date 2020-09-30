#pragma once

#ifndef LIGHTINGSYSTEM_H
#define LIGHTINGSYSTEM_H

#include <GL/glew.h>
#include "glhelper.h"
#include "Systems/ISystem.h"
#include "Systems/WindowsSystem.h"

class LightingSystem : public ISystem
{
	bool debug_;

	//we'll use this temporarily later on replace with components and stuff
	struct PointLight
	{
		GLint x_, y_;
		GLfloat red_, green_, blue_;
		GLfloat radius_;
		GLfloat intensity_;

		PointLight() = default;

		PointLight(GLint x, GLint y,
				   GLfloat red, GLfloat green, GLfloat blue,
				   GLfloat radius,
				   GLfloat intensity);
	};

	GLuint frame_buffer;
	GLuint render_buffer;
	GLuint final_texture;
	GLuint lighting_buffer;
	GLuint lighting_texture;

	WindowsSystem* windows_system;

public:

	void Init();
	void Update(float frametime);
	void TestDraw();
	void Cleanup();
	GLuint& GetFrameBuffer();
	GLuint& GetFinalTexture();
	GLuint& GetLightingTexture();

	//returns the name of the system for debug use
	std::string GetName();

	//function more akin to "What to do when message is received" for internal logic
	void SendMessageD(Message* m);
};

#endif