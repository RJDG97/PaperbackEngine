/**********************************************************************************
*\file         EffectsSystem.cpp
*\brief        Contains definition of functions and variables used for
*               the Effects System
*
*\author       Jun Pu, Lee, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
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
    
    spore_size_effect_.SetMaxSize({ 800.0f, 450.0f });
    spore_size_effect_.SetMinSize({ 400.0f, 225.0f });
    spore_size_effect_.effect_timer_ = 0.5f;
    spore_size_effect_.max_clear_ = { 320.0f, 180.0f };

    collectible_map_ = component_manager_->GetComponentArray<Collectible>();
}


void EffectsSystem::Update(float frametime)
{
    const EntityID player_id = entity_manager_->GetPlayerEntities() 
        ? entity_manager_->GetPlayerEntities()->GetID() 
        : 0;

    health_effect_.Update(frametime);
    color_effect_.Update(frametime, player_id);
    spore_size_effect_.Update(frametime, player_id);

    for (auto& [id, collectible] : *collectible_map_)
    {
        Name* name = component_manager_->GetComponent<Name>(id);
        Motion* motion = component_manager_->GetComponent<Motion>(id);
        PointLight* light = component_manager_->GetComponent<PointLight>(id);
        AnimationRenderer* anim = component_manager_->GetComponent<AnimationRenderer>(id);

        if (!motion || !anim || !name || !light || name->GetEntityName() != "Spore")
            continue;

        if (std::abs(motion->GetVelocity().x) > 0 || std::abs(motion->GetVelocity().y) > 0)
        {
            light->SetPulse(false);

            float opacity = anim->GetOpacity();
            float radius = light->GetRadius();
            float new_opacity = std::max(0.0f, opacity *= 0.99f);
            float new_radius = std::max(0.0f, radius *= 0.99f);

            anim->SetOpacity(new_opacity);
            light->SetRadius(new_radius);
        }
    }
}

void EffectsSystem::Reset()
{
    spore_size_effect_.SetMaxSize({ 800.0f, 450.0f });
    spore_size_effect_.SetMinSize({ 400.0f, 225.0f });
    spore_size_effect_.effect_timer_ = 0.5f;
    spore_size_effect_.max_clear_ = { 320.0f, 180.0f };

    spore_size_effect_.Initialize();
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


void VignetteSporeSizeEffect::SetStatus(float dur)
{
    current_ = dur;
}

void VignetteSporeSizeEffect::SetMinSize(glm::vec2 min)
{
    min_size_ = min;
    curr_size_ = min;
}

void VignetteSporeSizeEffect::SetMaxSize(glm::vec2 max)
{
    max_size_ = max;
}

void VignetteSporeSizeEffect::ResetSizeOnDeath()
{
    GraphicsSystem* graphics = &*CORE->GetSystem<GraphicsSystem>();

    curr_clear_ = { std::max(0.0f, curr_size_.x - max_clear_.x), 
                    std::max(0.0f, curr_size_.y - max_clear_.y) };

    graphics->SetMaxVignetteSize(curr_size_);
    graphics->SetVignetteSize(curr_clear_);
}

void VignetteSporeSizeEffect::Initialize()
{
    count_ = 0;

    ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();
    CMap<Collectible>* collectible_map = component_mgr->GetComponentArray<Collectible>();

    // Initialize counter
    for (auto& [id, collectible] : *collectible_map)
    {
        if (collectible->GetItemName() == "Spore")
            ++count_;
    }
}

// Update the transition system's "ResetTransition" function to include an additional flag
// which determines if there is a limitation to the size of the vignette from the EffectsSystem
void VignetteSporeSizeEffect::Update(const float& dt, const EntityID& id)
{
    UNREFERENCED_PARAMETER(id);
    GraphicsSystem* graphics = &*CORE->GetSystem<GraphicsSystem>();

    // Update vignette size
    if (current_ > 0.0f)
    {

        rate_ = ((max_size_ - min_size_) / static_cast<float>(count_)) / effect_timer_;

        curr_size_ += rate_ * dt;
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


void HealthEffect::SetStatus(float dur, bool decrease)
{
    effect_timer_ = current_ = dur;
    decrease_ = decrease;
}

void HealthEffect::Update(const float& dt)
{
    EntityManager* e_mgr = &*CORE->GetManager<EntityManager>();
    ComponentManager* c_mgr = &*CORE->GetManager<ComponentManager>();
    GraphicsSystem* graphics = &*CORE->GetSystem<GraphicsSystem>();

    ParentMap* parents = c_mgr->GetComponentArray<ParentChild>();
    EntityID player_id = e_mgr->GetPlayerEntities() ? e_mgr->GetPlayerEntities()->GetID() : 0;

    // Return if invalid
    if (current_ < 0.0f) return;

    for (auto& [parent_id, parent] : *parents)
    {
        Name* name = c_mgr->GetComponent<Name>(parent_id);

        if (name->GetEntityName() != "Watergauge") continue;

        TextureRenderer* texture = c_mgr->GetComponent<TextureRenderer>(parent_id);
        Transform* xform = c_mgr->GetComponent<Transform>(parent_id);
        Scale* scale = c_mgr->GetComponent<Scale>(parent_id);
        Health* health = c_mgr->GetComponent<Health>(player_id);

        if (!xform || !scale || !health || !texture) return;

        int curr = health->GetCurrentHealth();
        int max = health->GetMaxHealth();

        Vector2D u_rate = (scale->GetScale() / static_cast<float>(max)) / (effect_timer_);
        u_rate.y = 0.0f;

        if (curr > max / 2.0f) {
            graphics->ChangeTexture(texture, "UI_HealthBar_Full_0");
        }
        else if (curr <= max / 2.0f && curr > max / 3.0f) {
            graphics->ChangeTexture(texture, "UI_HealthBar_Half_0");
        }
        else {
            graphics->ChangeTexture(texture, "UI_HealthBar_Low_0");
        }

        Vector2D new_offset = xform->GetOffset() + ((decrease_ ? -u_rate : u_rate) * dt) / CORE->GetGlobalScale();

        xform->SetOffset(new_offset);
    }

    current_ -= dt;
}


/*

        ComponentManager* c_mgr = &*CORE->GetManager<ComponentManager>();
        GraphicsSystem* graphics = &*CORE->GetSystem<GraphicsSystem>();
        Name* name = c_mgr->GetComponent<Name>(parent_id);

        if (name->GetEntityName() != "Watergauge") return;

        TextureRenderer* texture = c_mgr->GetComponent<TextureRenderer>(parent_id);
        Transform* xform = c_mgr->GetComponent<Transform>(parent_id);
        Scale* scale = c_mgr->GetComponent<Scale>(parent_id);
        Health* health = c_mgr->GetComponent<Health>(player_id);

        if (!xform || !scale || !health || !texture) return;

        int curr = health->GetCurrentHealth();
        int max = health->GetMaxHealth();

        if (curr > max / 2.0f) {
            graphics->ChangeTexture(texture, "UI_HealthBar_Full_0");
        }
        else if (curr <= max / 2.0f && curr > max / 3.0f) {
            graphics->ChangeTexture(texture, "UI_HealthBar_Half_0");
        }
        else {
            graphics->ChangeTexture(texture, "UI_HealthBar_Low_0");
        }

        int mod = max - curr;
        Vector2D hp_scale = scale->GetScale() / static_cast<float>(max);
        hp_scale.y = 0.0f;

        xform->SetOffset(static_cast<float>(mod) * -hp_scale / CORE->GetGlobalScale());
    }

*/