#include "Components/AnimationRenderer.h"
#include "Components/Transform.h"
#include "Components/Scale.h"
#include "Engine/Core.h"
#include "Systems/GraphicsSystem.h"
#include <glm/gtc/type_ptr.hpp>

#define M_PI       3.14159265358979323846

AnimationRenderer::AnimationRenderer() {

}

AnimationRenderer::~AnimationRenderer() {  
    
    CORE->GetSystem<GraphicsSystem>()->RemoveAnimationRendererComponent(Component::GetOwner()->GetID());
}

void AnimationRenderer::Init() {
    
    CORE->GetSystem<GraphicsSystem>()->AddAnimationRendererComponent(Component::GetOwner()->GetID(), this);
}

void AnimationRenderer::Serialize(std::stringstream& data) {

    std::cout << "Serializing AnimationRenderer" << std::endl;

    std::string model;
    std::string shdr_pgm;
    int num_animations;

    data >> model
         >> shdr_pgm
         >> num_animations;

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

    model_ = CORE->GetManager<ModelManager>()->GetModel(model);
    shdr_pgm_ = CORE->GetManager<ShaderManager>()->GetShdrpgm(shdr_pgm);
}