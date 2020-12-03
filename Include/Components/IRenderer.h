/**********************************************************************************
*\file         IRenderer.h
*\brief        Contains the interface for the IRenderer Component
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _IOBJECTRENDERER_H_
#define _IOBJECTRENDERER_H_

#include <windows.h>
#include "Entity/Entity.h"
#include "Manager/ModelManager.h"
#include "Manager/ShaderManager.h"

class IRenderer : public Component {

protected:

	bool x_mirror_ = false;
	bool y_mirror_ = false;

	std::string texture_name_;
	GLuint texture_handle_;
	std::vector<glm::vec2> tex_vtx_;
	
	int layer_;
	bool ui_; //if object is on UI or in the world

	bool alive_;

public:

	friend class GraphicsSystem;
	friend class ParticleSystem;
	friend class Emitter;

	bool IsAlive() { return alive_; }
	void SetAlive(bool life) { alive_ = life; }

};

#endif