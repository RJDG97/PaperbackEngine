#pragma once

#include <GL/glew.h>
#include "glhelper.h"

class LightingSystem
{
	GLuint lighting_buffer;
	GLuint frame_buffer;
	GLuint render_buffer;
	GLuint final_texture;

public:

	void Init();
	void Update();
	void Cleanup();
	GLuint GetFrameBuffer();
	GLuint GetRenderBuffer();
	GLuint GetFinalTexture();
};