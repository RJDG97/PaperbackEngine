#include "Manager\BatchManager.h"
#pragma once

void BatchManager::Init()
{

}

void BatchManager::CreateBatch()
{
	GLuint vbo_hdl;
	glCreateBuffers(1, &vbo_hdl);

	//Render 500 objects at once
	glNamedBufferStorage(vbo_hdl, sizeof(glm::vec2) * 4 * 500 +
						 sizeof(glm::vec2) * 4 * 500 + sizeof(glm::vec2) * 4 * 3 * 500, //3 -> pos, scal, rot
						 nullptr, GL_DYNAMIC_STORAGE_BIT);
}
