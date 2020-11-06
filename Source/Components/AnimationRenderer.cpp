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
    //CORE->GetManager<ComponentManager>()->RemoveComponent<AnimationRenderer>(Component::GetOwner()->GetID());
}

void AnimationRenderer::Init() {
    
    CORE->GetSystem<GraphicsSystem>()->AddAnimationRendererComponent(Component::GetOwner()->GetID(), this);
    
    for (int i = 0; i < animation_names_.size(); ++i)
    {
        CORE->GetSystem<GraphicsSystem>()->AddAnimation(this, animation_names_[i]);
    }
    
    CORE->GetSystem<GraphicsSystem>()->SetAnimation(this, current_animation_name_);
    texture_handle_ = current_animation_->GetAnimationFramesHandle();
    tex_vtx_ = *current_animation_->GetTexVtx();
}

void AnimationRenderer::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

    (void)writer;
    /*
    writer->StartObject();

    writer->Key("component");
    writer->String("AABB");

    writer->Key("scale");
    writer->String((std::to_string(scale_.x) + " " + std::to_string(scale_.y)).c_str());

    writer->EndObject();*/
}

void AnimationRenderer::DeSerialize(std::stringstream& data) {

    std::cout << "Serializing AnimationRenderer" << std::endl;

    int num_animations;

    data >> num_animations;

    for (int i = 0; i < num_animations; ++i) {

        std::string temp;
        data >> temp;

        animation_names_.push_back(temp);
        //won't need this after serialize is moved to GraphicsSystem
        //CORE->GetSystem<GraphicsSystem>()->AddAnimation(this, animation_name);
    }

    data >> current_animation_name_;

    data >> play_animation_ >> has_finished_animating_ >> layer_;
}

std::shared_ptr<Component> AnimationRenderer::Clone() {
    M_DEBUG->WriteDebugMessage("Cloning AnimationRenderer Component\n");

	std::shared_ptr<AnimationRenderer> cloned = std::make_shared<AnimationRenderer>();

    // IRenderer
    cloned->layer_ = layer_;

    // AnimationRenderer
	cloned->obj_animations_ = obj_animations_; // Might need copy ctor
    cloned->animation_names_ = animation_names_;
    cloned->current_animation_name_ = current_animation_name_;
	cloned->current_animation_ = current_animation_;
	cloned->play_animation_ = play_animation_;
    cloned->has_finished_animating_ = has_finished_animating_;

	return cloned;
}