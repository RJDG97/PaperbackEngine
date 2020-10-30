#pragma once
#ifndef _IUIRENDERER_H_
#define _IUIRENDERER_H_

#include <windows.h>
#include "Entity/Entity.h"
#include "Manager/ModelManager.h"
#include "Manager/ShaderManager.h"

class IUIObjectRenderer : public Component {

protected:

	GLuint* texture_handle_;
	int layer_;

public:

	friend class GraphicsSystem;

};

#endif
