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

struct VignetteSporeSizeEffect
{
    float effect_timer_, current_;
    glm::vec2 max_clear_, curr_clear_;
    glm::vec2 min_size_, max_size_, curr_size_;

    glm::vec2 rate_;
    bool decrease_;
    int count_;

    void SetStatus(float dur);
    void SetMinSize(glm::vec2 min);
    void SetMaxSize(glm::vec2 max);

    void Initialize();
    void Update(const float& dt, const EntityID& id = 0);
};

struct HealthEffect
{
    using ParentMap = CMap<ParentChild>;

    float effect_timer_, current_;
    float rate_;
    bool decrease_;

    void SetStatus(float dur, bool decrease = true);

    void Update(const float& dt);
};


class EffectsSystem : public ISystem
{
public:

    using CollectibleMap = CMap<Collectible>;

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
      \fn Reset()

      \brief Reset all values to default
    */
    /******************************************************************************/
    void Reset();

    /******************************************************************************/
    /*!
      \fn SendMessageD()

      \brief Handles incoming messages and sorts based on message id
    */
    /******************************************************************************/
    void SendMessageD(Message* m) {
        (void)m;
    }

    HealthEffect health_effect_;
    VignetteColorEffect color_effect_;
    VignetteSporeSizeEffect spore_size_effect_;

private:

    CollectibleMap* collectible_map_;
    EntityManager* entity_manager_;
    ComponentManager* component_manager_;
    GraphicsSystem* graphics_system_;
};


#endif