/**********************************************************************************
*\file         IRenderer.h
*\brief        Contains the interface for the IRenderer Component
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
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

	int order_in_layer_;
	int layer_;
	bool alive_;
	float opacity_;

public:

	friend class GraphicsSystem;
	friend class ParticleSystem;
	friend class Emitter;

    IRenderer(): order_in_layer_{0}, layer_{0}, alive_{1}, opacity_{1} {}

/******************************************************************************/
/*!
  \fn IsAlive()

  \brief Return alive status
*/
/******************************************************************************/
	bool IsAlive() { return alive_; }

/******************************************************************************/
/*!
  \fn SetAlive()

  \brief Set alive status
*/
/******************************************************************************/
	void SetAlive(bool life) { alive_ = life; }

/******************************************************************************/
/*!
	\fn GetLayer()

	\brief Get layer
*/
/******************************************************************************/
	int GetLayer() { return layer_; }

/******************************************************************************/
/*!
	\fn GetLayer()

	\brief Get layer
*/
/******************************************************************************/
	void SetLayer(int layer) { layer_ = layer; }

/******************************************************************************/
/*!
	\fn GetLayer()

	\brief Get layer
*/
/******************************************************************************/
	int GetOrderInLayer() { return order_in_layer_; }

/******************************************************************************/
/*!
	\fn GetOpacity()

	\brief Get opacity
*/
/******************************************************************************/

	float GetOpacity() { return opacity_; }

	/******************************************************************************/
	/*!
		\fn GetOpacity()

		\brief Get opacity
	*/
	/******************************************************************************/

	void SetOpacity(float opacity) { opacity_ = opacity; }
};

#endif