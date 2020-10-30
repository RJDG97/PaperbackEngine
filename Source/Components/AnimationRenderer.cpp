#include "Components/AnimationRenderer.h"
#include "Components/Transform.h"
#include "Components/Scale.h"
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
    texture_handle_ = current_animation_->GetAnimationFramesHandle();
    tex_vtx_initial_ = *current_animation_->GetTexVtx();
    tex_vtx_mirrored_ = std::vector<glm::vec2*>{ &tex_vtx_initial_[0],
                                                 &tex_vtx_initial_[1],
                                                 &tex_vtx_initial_[2],
                                                 &tex_vtx_initial_[3], };
    tex_vtx_sent_ = *current_animation_->GetTexVtx();
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

        std::string animation_name;
        data >> animation_name;
        //won't need this after serialize is moved to GraphicsSystem
        CORE->GetSystem<GraphicsSystem>()->AddAnimation(this, animation_name);
    }

    std::string current_animation_name;
    data >> current_animation_name;
    CORE->GetSystem<GraphicsSystem>()->SetAnimation(this, current_animation_name);

    data >> play_animation_ >> has_finished_animating_ >> layer_;
}

std::shared_ptr<Component> AnimationRenderer::Clone() {
    M_DEBUG->WriteDebugMessage("Cloning AnimationRenderer Component\n");

	std::shared_ptr<AnimationRenderer> cloned = std::make_shared<AnimationRenderer>();

    // IRenderer
    cloned->layer_ = layer_;

    // AnimationRenderer
	cloned->obj_animations_ = obj_animations_; // Might need copy ctor
	cloned->current_animation_ = current_animation_;
	cloned->play_animation_ = play_animation_;
    cloned->has_finished_animating_ = has_finished_animating_;

	return cloned;
}