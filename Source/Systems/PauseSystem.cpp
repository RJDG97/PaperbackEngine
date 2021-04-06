/**********************************************************************************
*\file         PauseSystem.cpp
*\brief        Contains definition of functions and variables used for
*               the Parenting System
*
*\author       Jun Pu, Lee, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Engine/Core.h"
#include "Systems/PauseSystem.h"


void PauseSystem::Init()
{
    mgr = CORE->GetManager<ComponentManager>();
    click_map_ = mgr->GetComponentArray<Clickable>();
    pause_layering_.clear();
    previous_layer_ = 0;
    active_layer_ = 1;
    ended_ = false;
}


void PauseSystem::Update(float frametime)
{
    // Note: Expecting that every single button contains a Texture Renderer Component

    // If a layer has been toggled on (Not -1)
    if (active_layer_ > 0)
    {
        // Retrieve the current layer to be updated
        decltype(auto) new_layer = pause_layering_[active_layer_];

        // Enable the next layer
        for (auto id : new_layer)
        {
            mgr->GetComponent<Clickable>(id)->SetActive(true);
            mgr->GetComponent<TextureRenderer>(id)->SetAlive(true);
        }

        // Disable the previous "Active" layer
        if (previous_layer_ > 0)
        {
            decltype(auto) prev_layer = pause_layering_[previous_layer_];

            for (auto id : prev_layer)
            {
                mgr->GetComponent<Clickable>(id)->SetActive(false);
                mgr->GetComponent<TextureRenderer>(id)->SetAlive(false);
            }
        }

        // Reset both indexes (Do not place outside the if check)
        previous_layer_ = active_layer_;
        active_layer_ = -1;
    }
}

// Do avoid using this function if possible (I guess)
// Use EnableNextLayer && DisablePreviousLayer
void PauseSystem::SetActiveLayer(const int& layer)
{
    active_layer_ = layer;
}

void PauseSystem::EnableNextLayer()
{
    active_layer_ = previous_layer_ + 1;
}

bool PauseSystem::RevertPreviousLayer()
{
    if (previous_layer_ - 1 >= 1)
    {
        active_layer_ = previous_layer_ - 1;
        return false;
    }
    active_layer_ = 0;
    return true;
}

void PauseSystem::InitializeClickables()
{
    // Remove previous level's clickables
    ClearSystem();

    for (auto& [id, click] : *click_map_)
    {
        pause_layering_[ click->group_ ].push_back( id );
    }
}

void PauseSystem::ClearSystem()
{
    ended_ = false;
    previous_layer_ = 0;
    active_layer_ = 1;
    pause_layering_.clear();
}

const int& PauseSystem::PrevLayer() const
{
    return previous_layer_;
}

void PauseSystem::TerminateState(bool status)
{
    ended_ = status;
}

bool PauseSystem::GetState() const
{
    return ended_;
}
