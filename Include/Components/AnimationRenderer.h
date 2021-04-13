/**********************************************************************************
*\file         AnimationRenderer.h
*\brief        Contains declaration of functions and variables used for
*			   the AnimationRenderer Component
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once

#ifndef _ANIMATIONRENDERER_H_
#define _ANIMATIONRENDERER_H_

#include <windows.h>
#include "Entity/Entity.h"
#include "Manager/ModelManager.h"
#include "Manager/AnimationManager.h"
#include "Manager/ShaderManager.h"
#include "Components/SpriteRenderer.h"
#include <glm/glm.hpp>

class AnimationRenderer : public SpriteRenderer {

	std::vector<std::string> animation_names_;
	std::map<std::string, Animation> obj_animations_;	// all possible animations that an object can switch between
	std::string current_animation_name_;
	Animation* current_animation_;						// current animation playing (points to an animation inside obj_animations_)

	bool play_animation_;								// true - play animation, false - stop playing animation
	bool reversed_;										// animation is played in reverse order
	bool has_finished_animating_;						// true - animation has not finished, false - still in middle of animation
	float animation_speed_;								// speed of animation

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
  \fn Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)

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
  \fn SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)

  \brief Serialises a component as a clone into JSON format
*/
/******************************************************************************/
	void SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) override;

/******************************************************************************/
/*!
	\fn DeSerializeClone(std::stringstream& data)

	\brief Serializes data members within the Component
*/
/******************************************************************************/
	void DeSerializeClone(std::stringstream& data) override;

/******************************************************************************/
/*!
  \fn Clone()

  \brief Clones the existing component
*/
/******************************************************************************/
	std::shared_ptr<Component> Clone() override;

/******************************************************************************/
/*!
  \fn GetCurrentAnimation()

  \brief Get the current animation name
*/
/******************************************************************************/
	std::string GetCurrentAnimation();

/******************************************************************************/
/*!
  \fn GetAvailableAnimation()

  \brief Get the possible animations that the entity could use
*/
/******************************************************************************/
	std::map<std::string, Animation> GetAvailableAnimation();
	
/******************************************************************************/
/*!
	\fn FinishedAnimating()

	\brief Checks if the animation has reached and finished its last frame
*/
/******************************************************************************/
	bool FinishedAnimating() const;

/******************************************************************************/
/*!
	\fn SetAnimationStatus()

	\brief Sets animation to pause/play
*/
/******************************************************************************/
	void SetAnimationStatus(bool status);

/******************************************************************************/
/*!
	\fn GetReversed()

	\brief Gets the reversed bool
*/
/******************************************************************************/
	bool GetReversed() { return reversed_; }

/******************************************************************************/
/*!
	\fn SetReversed()

	\brief Sets the reversed bool
*/
/******************************************************************************/
	void SetReversed(bool value) { reversed_ = value; }

/******************************************************************************/
/*!
	\fn GetAnimationSpeed()

	\brief Gets the animation speed
*/
/******************************************************************************/
	float GetAnimationSpeed() { return animation_speed_; }

/******************************************************************************/
/*!
	\fn SetAnimationSpeed()

	\brief Sets the animation speed
*/
/******************************************************************************/
	void SetAnimationSpeed(float value) { animation_speed_ = value; }
};

#endif