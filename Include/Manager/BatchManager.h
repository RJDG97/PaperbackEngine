#pragma once
#ifndef _BATCH_MANAGER_H_
#define _BATCH_MANAGER_H_

#include "Manager/IManager.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Batch
{
	GLuint vbo_hdl_;
};

class BatchManager : public IManager {

	std::vector<Batch> batches_;

public:

	void Init() override;

	void CreateBatch();

};

#endif