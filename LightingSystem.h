#pragma once

#include <GL/glew.h>
#include "glhelper.h"

class LightingSystem
{
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

public:

	void Init();
	void Update();
	void Draw();
	void Cleanup();
	GLuint& GetFrameBuffer();
	GLuint& GetFinalTexture();
	GLuint& GetLightingTexture();
};