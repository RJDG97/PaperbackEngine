#pragma once
#ifndef _SPRITERENDERER_H_
#define _SPRITERENDERER_H_

#include <windows.h>
#include "Entity/Entity.h"
#include "Components/IRenderer.h"
#include "Manager/ModelManager.h"
#include "Manager/ShaderManager.h"

class SpriteRenderer : public IRenderer {

protected:

	bool x_mirror_ = false;
	bool y_mirror_ = false;

	GLuint texture_handle_;				//handle to the texture
	std::vector<glm::vec2> tex_vtx_;	//texture vertices

	glm::vec3 tint_;

    SpriteRenderer(): tint_{1.0f, 1.0f, 1.0f} {}

public:

	friend class GraphicsSystem;

};

#endif