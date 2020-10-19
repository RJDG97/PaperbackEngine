#pragma once

#ifndef _ANIMATIONRENDERER_H_
#define _ANIMATIONRENDERER_H_

#include <windows.h>
#include "Entity/Entity.h"
#include "Manager/ModelManager.h"
#include "Manager/AnimationManager.h"
#include "Manager/ShaderManager.h"
#include "Components/IRenderer.h"
#include <glm/glm.hpp>

class AnimationRenderer : public IRenderer {

	std::map<std::string, Animation> obj_animations_;	// all possible animations that an object can switch between
	Animation* current_animation_;						// current animation playing (points to an animation inside obj_animations_)

	bool play_animation_;								// true - play animation, false - stop playing animation
	bool has_finished_animating_;						// true - animation has not finished, false - still in middle of animation

	float time_elapsed_ = 0.0f;							// time elapsed this frame
	float total_time_elapsed_ = 0.0f;					// time elapsed this cycle

public:

	friend class GraphicsSystem;

/******************************************************************************/
/*!
	\fn AnimationRenderer()

	\brief Constructor for AnimationRenderer that defaults the data members of
		   the component
*/
/******************************************************************************/

	AnimationRenderer();

/******************************************************************************/
/*!
	\fn ~AnimationRenderer()

	\brief Destructor for AnimationRenderer that removes the component from the
		 Graphics System's animation renderer map
*/
/******************************************************************************/
	~AnimationRenderer();

/******************************************************************************/
/*!
	\fn Init()

	\brief Adds the component itself to the Graphics System's animation
		   renderer map
*/
/******************************************************************************/
	void Init();

/******************************************************************************/
/*!
  \fn Serialize()

  \brief Serialises a component into JSON format
*/
/******************************************************************************/
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) override;

/******************************************************************************/
/*!
	\fn DeSerialize(std::stringstream& data)

	\brief Reads data from a stringstream and stores them into the data members
*/
/******************************************************************************/
	void DeSerialize(std::stringstream& data) override;

/******************************************************************************/
/*!
  \fn Clone()

  \brief Clones the existing component
*/
/******************************************************************************/
	std::shared_ptr<Component> Clone() override;
};

#endif