#pragma once

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <windows.h>
#include "IRenderer.h"
#include "Entity/Entity.h"
#include "Manager/TextureManager.h"
#include <glm/glm.hpp>

class Renderer : public IRenderer {

	Texture texture_;

public:

	friend class GraphicsSystem;

	Renderer();
	~Renderer();

	void Init();

	void Serialize(std::stringstream& data) override;
};

#endif