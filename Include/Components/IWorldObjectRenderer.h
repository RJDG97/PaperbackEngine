#pragma once
#ifndef _IOBJECTRENDERER_H_
#define _IOBJECTRENDERER_H_

#include <windows.h>
#include "Entity/Entity.h"
#include "Manager/ModelManager.h"
#include "Manager/ShaderManager.h"

class IWorldObjectRenderer : public Component {

protected:

	bool x_mirror_ = false;
	bool y_mirror_ = false;

	std::string texture_name_;
	GLuint* texture_handle_;
	std::vector<glm::vec2> tex_vtx_initial_;	//initial texture vertex (before flipping)
	std::vector<glm::vec2*> tex_vtx_mirrored_;	//mirrored texture vertex (after flipping the initial)
	std::vector<glm::vec2> tex_vtx_sent_;		//texture vertex sent to the shaders
	int layer_;

public:

	friend class GraphicsSystem;

};

#endif