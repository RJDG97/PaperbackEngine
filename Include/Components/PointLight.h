#pragma once

#ifndef _POINTLIGHT_H_
#define _POINTLIGHT_H_

#include <windows.h>
#include "Entity/Entity.h"
#include "Manager/ModelManager.h"
#include "Manager/ShaderManager.h"
#include "MathLib/Vector2D.h"

class PointLight : public Component {

	Model model_;
	Shader shdr_pgm_;

	glm::vec3 color_;
	float radius_;
	float intensity_;

	glm::vec2 pos_;

public:

	friend class LightingSystem;

	PointLight();
	~PointLight();

	void Init();
	void Serialize(std::stringstream& data) override;
};

#endif