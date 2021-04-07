/**********************************************************************************
*\file         TransitionSystem.h
*\brief        Contains definition of functions and variables used for
*               the Effects System
*
*\author       Jun Pu, Lee, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#ifndef _EFFECTS_SYSTEM_H_
#define _EFFECTS_SYSTEM_H_

#include "Systems/ISystem.h"
#include "Manager/EntityManager.h"
#include "Manager/ComponentManager.h"
#include "Systems/GraphicsSystem.h"


struct VignetteColorEffect
{
    bool active_{false};
    float effect_timer_, current_;
    glm::vec3 start_color_, end_color_;
    glm::vec3 rate_;

    void SetTimer(float duration);
    void SetEndVignetteColor(glm::vec3 end_color);
    void SetStartVignetteColor(glm::vec3 start_color);

    void Initialize();
    void Update(const float& dt, const EntityID& id = 0);
};

struct VignetteSizeEffect
{
    float effect_timer_, current_;
    glm::vec2 max_clear_, curr_clear_;
    glm::vec2 min_size_, max_size_, curr_size_;

    glm::vec2 rate_;
    bool decrease_;

    void SetStatus(float dur, bool decrease = true);
    void SetMinSize(glm::vec2 min);
    void SetMaxSize(glm::vec2 max);

    void Update(const float& dt, const EntityID& id = 0);
};


class EffectsSystem : public ISystem
{
public:

    /******************************************************************************/
    /*!
      \fn Init()

      \brief Used to initialise a system if required;
    */
    /******************************************************************************/
    void Init();

    /******************************************************************************/
    /*!
      \fn Update()

      \brief Used to run logic that is required every game loop.
    */
    /******************************************************************************/
    void Update(float frametime);
    
    /******************************************************************************/
    /*!
      \fn GetName()

      \brief Returns the name of the system
    */
    /******************************************************************************/
    std::string GetName() { return "EffectsSystem"; }

    /******************************************************************************/
    /*!
      \fn SendMessageD()

      \brief Handles incoming messages and sorts based on message id
    */
    /******************************************************************************/
    void SendMessageD(Message* m) {
        (void)m;
    }

    VignetteColorEffect color_effect_; // collision and game
    VignetteSizeEffect size_effect_; // only here

private:


    EntityManager* entity_manager_;
    ComponentManager* component_manager_;
    GraphicsSystem* graphics_system_;
};


#endif