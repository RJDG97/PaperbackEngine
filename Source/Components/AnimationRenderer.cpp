/**********************************************************************************
*\file         AnimationRenderer.cpp
*\brief        Contains definition of functions and variables used for
*			   the AnimationRenderer Component
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Components/AnimationRenderer.h"
#include "Components/Transform.h"
#include "Components/Scale.h"
#include "Manager/ComponentManager.h"
#include "Engine/Core.h"
#include "Systems/GraphicsSystem.h"

#define M_PI       3.14159265358979323846

AnimationRenderer::AnimationRenderer() {

}

AnimationRenderer::~AnimationRenderer() {  
    
    CORE->GetSystem<GraphicsSystem>()->RemoveAnimationRendererComponent(Component::GetOwner()->GetID());
}

void AnimationRenderer::Init() {
    
    CORE->GetSystem<GraphicsSystem>()->AddAnimationRendererComponent(Component::GetOwner()->GetID(), this);
    
    for (int i = 0; i < animation_names_.size(); ++i) {

        CORE->GetSystem<GraphicsSystem>()->AddAnimation(this, animation_names_[i]);
    }
    
    CORE->GetSystem<GraphicsSystem>()->SetAnimation(this, current_animation_name_);
    texture_handle_ = current_animation_->GetAnimationFramesHandle();
    tex_vtx_ = *current_animation_->GetTexVtx();
}

void AnimationRenderer::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {
    
    writer->StartObject();

    writer->Key("component");
    writer->String("AnimationRenderer");

    writer->Key("number of animations");
    writer->String((std::to_string(animation_names_.size()).c_str()));
    
    for (auto it = animation_names_.begin(); it != animation_names_.end(); ++it) {

        writer->Key("add animation");
        writer->String(it->c_str());
    }

    writer->Key("set animation");
    writer->String(current_animation_name_.c_str());

    writer->Key("play_animation");
    writer->String((std::to_string(play_animation_).c_str()));

    writer->Key("has_finished_animating");
    writer->String((std::to_string(has_finished_animating_).c_str()));

    writer->Key("layer");
    writer->String((std::to_string(layer_).c_str()));

    writer->Key("ui");
    writer->String(std::to_string(ui_).c_str());

    writer->Key("alive");
    writer->String(std::to_string(alive_).c_str());

    writer->EndObject();
}

void AnimationRenderer::DeSerialize(std::stringstream& data) {

    std::cout << "Serializing AnimationRenderer" << std::endl;

    int num_animations;

    data >> num_animations;

    for (int i = 0; i < num_animations; ++i) {

        std::string temp;
        data >> temp;

        animation_names_.push_back(temp);
    }

    data >> current_animation_name_;

    data >> play_animation_ >> has_finished_animating_ >> layer_ >> ui_ >> alive_;
}

void AnimationRenderer::DeSerializeClone(std::stringstream& data)
{
    DeSerialize(data);
}

std::shared_ptr<Component> AnimationRenderer::Clone() {
    M_DEBUG->WriteDebugMessage("Cloning AnimationRenderer Component\n");

	std::shared_ptr<AnimationRenderer> cloned = std::make_shared<AnimationRenderer>();

    // IRenderer
    cloned->layer_ = layer_;

    // AnimationRenderer
	cloned->obj_animations_ = obj_animations_;
    cloned->animation_names_ = animation_names_;
    cloned->current_animation_name_ = current_animation_name_;
	cloned->current_animation_ = current_animation_;
	cloned->play_animation_ = play_animation_;
    cloned->has_finished_animating_ = has_finished_animating_;
    cloned->ui_ = ui_;
    cloned->alive_ = alive_;

	return cloned;
}

std::string AnimationRenderer::GetCurrentAnimation() {

    return current_animation_name_;
}

bool AnimationRenderer::FinishedAnimating() const {

    return has_finished_animating_;
}
