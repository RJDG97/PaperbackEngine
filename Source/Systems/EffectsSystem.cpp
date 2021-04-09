/**********************************************************************************
*\file         EffectsSystem.cpp
*\brief        Contains definition of functions and variables used for
*               the Effects System
*
*\author       Jun Pu, Lee, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Engine/Core.h"
#include "Systems/EffectsSystem.h"


void EffectsSystem::Init()
{
    entity_manager_ = &*CORE->GetManager<EntityManager>();
    component_manager_ = &*CORE->GetManager<ComponentManager>();
    graphics_system_ = &*CORE->GetSystem<GraphicsSystem>();

    size_effect_.SetMaxSize({ 800.0f, 450.0f });
    size_effect_.SetMinSize({ 400.0f, 225.0f });
    size_effect_.effect_timer_ = 0.5f;
    size_effect_.max_clear_ = { 320.0f, 180.0f };
}


void EffectsSystem::Update(float frametime)
{
    const EntityID player_id = entity_manager_->GetPlayerEntities() 
        ? entity_manager_->GetPlayerEntities()->GetID() 
        : 0;

    color_effect_.Update(frametime, player_id);
    size_effect_.Update(frametime, player_id);
}





void VignetteColorEffect::SetTimer(float duration)
{
    current_ = effect_timer_ = duration;
    active_ = duration > 0.0f ? true : false;
}

void VignetteColorEffect::SetEndVignetteColor(glm::vec3 end_color)
{
    end_color_ = end_color;
    Initialize();
}

void VignetteColorEffect::SetStartVignetteColor(glm::vec3 start_color)
{
    start_color_ = start_color;
}

void VignetteColorEffect::Initialize()
{
    rate_ = (end_color_ - start_color_) / effect_timer_;
}

void VignetteColorEffect::Update(const float& dt, const EntityID& id)
{
    GraphicsSystem* graphics = &*CORE->GetSystem<GraphicsSystem>();

    if (active_)
    {
        if (effect_timer_ == current_)
            graphics->SetVignetteColor(start_color_);
        graphics->SetVignetteColor(graphics->GetVignetteColor() + rate_ * dt);

        current_ -= dt;
        current_ = effect_timer_ < 0.0f ? false : true;
    }
    (void)id;
}





void VignetteSizeEffect::SetStatus(float dur, bool decrease)
{
    current_ = dur;
    decrease_ = decrease;
}

void VignetteSizeEffect::SetMinSize(glm::vec2 min)
{
    min_size_ = min;
}

void VignetteSizeEffect::SetMaxSize(glm::vec2 max)
{
    max_size_ = max;
    curr_size_ = max;
}

void VignetteSizeEffect::Update(const float& dt, const EntityID& id)
{

    UNREFERENCED_PARAMETER(id);
    GraphicsSystem* graphics = &*CORE->GetSystem<GraphicsSystem>();
    //ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();

    // Update vignette size
    if (current_ > 0.0f)
    {

        rate_ = ((max_size_ - min_size_) * 0.1f) / effect_timer_;
        rate_ = decrease_ ? rate_ : -rate_;

        curr_size_ -= rate_ * dt;
        curr_clear_ = { std::max(0.0f, curr_size_.x - max_clear_.x), std::max(0.0f, curr_size_.y - max_clear_.y) };

        if (curr_clear_.x > max_clear_.x && curr_clear_.y > max_clear_.y)
        {
            curr_clear_ = max_clear_;
        }

        graphics->SetMaxVignetteSize(curr_size_);
        graphics->SetVignetteSize(curr_clear_);

        current_ -= dt;
    }
}