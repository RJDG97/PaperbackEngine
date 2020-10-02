#include "Components/AnimationRenderer.h"
#include "Components/Transform.h"
#include "Components/Scale.h"
#include "Engine/Core.h"
#include "Systems/GraphicsSystem.h"
#include <glm/gtc/type_ptr.hpp>

#define M_PI       3.14159265358979323846

AnimationRenderer::AnimationRenderer()
{

}

AnimationRenderer::~AnimationRenderer()
{  
    CORE->GetSystem<GraphicsSystem>()->RemoveAnimationRendererComponent(Component::GetOwner()->GetID());
}

void AnimationRenderer::Init()
{
    CORE->GetSystem<GraphicsSystem>()->AddAnimationRendererComponent(Component::GetOwner()->GetID(), this);
}

void AnimationRenderer::PublishResults()
{
}

void AnimationRenderer::AddAnimation(std::string animation_name)
{
    obj_animations_[animation_name] = CORE->GetManager<AnimationManager>()->GetAnimation(animation_name);
    current_animation_ = obj_animations_[animation_name];
    current_frame_ = obj_animations_[animation_name]->GetFirstFrame();
}

void AnimationRenderer::SetAnimation(std::string animation_name)
{
    current_frame_ = obj_animations_[animation_name]->GetFirstFrame();
    current_animation_ = obj_animations_[animation_name];
    time_elapsed_ = 0.0f;
}

void AnimationRenderer::Update(float frametime, glm::mat3 world_to_ndc_xform)
{
    glm::mat3 scaling, rotation, translation;

    Vector2D scale = dynamic_cast<Scale*>(Component::GetOwner()->GetComponent(ComponentTypes::SCALE))->GetScale();

    scaling = glm::mat3{ scale.x, 0.0f, 0.0f,
                         0.0f, scale.y, 0.0f,
                         0.0f, 0.0f, 1.0f };

    Transform* transform = dynamic_cast<Transform*>(GetOwner()->GetComponent(ComponentTypes::TRANSFORM));
    assert(transform);

    float orientation = transform->rotation_;
    Vector2D position = transform->position_;

    rotation = glm::mat3{  glm::cos(orientation * M_PI / 180), glm::sin(orientation * M_PI / 180), 0.0f,
                          -glm::sin(orientation * M_PI / 180), glm::cos(orientation * M_PI / 180), 0.0f,
                           0.0f, 0.0f, 1.0f };

    translation = glm::mat3{ 1.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f,
                       position.x, position.y, 1.0f };

    mdl_to_ndc_xform_ = world_to_ndc_xform * translation * rotation * scaling;

    if (play_animation_ == true)
    {
        time_elapsed_ += frametime;
        has_finished_animating_ = false;

        if (time_elapsed_ >= current_animation_->GetFrameDuration())
        {
            ++current_frame_;

            if (current_frame_ == current_animation_->GetLastFrame())
            {
                has_finished_animating_ = true;
                current_frame_ = current_animation_->GetFirstFrame();
            }

            time_elapsed_ = 0.0f;
        }
    }
}

void AnimationRenderer::Draw()
{
    shdr_pgm_.Use();
    glBindVertexArray(model_.vaoid_);

    glBindTexture(GL_TEXTURE_2D, **current_frame_);
    glBindTextureUnit(0, **current_frame_);
    glUseProgram(shdr_pgm_.GetHandle());
    GLuint tex_loc = glGetUniformLocation(shdr_pgm_.GetHandle(), "uTex2d");
    glUniform1i(tex_loc, 0);

    GLint uniform_var_loc1 =
        glGetUniformLocation(shdr_pgm_.GetHandle(), "uModel_to_NDC");

    if (uniform_var_loc1 >= 0) {
        glUniformMatrix3fv(uniform_var_loc1, 1, GL_FALSE,
            glm::value_ptr(mdl_to_ndc_xform_));
    }

    glDrawElements(GL_TRIANGLE_STRIP, model_.draw_cnt_, GL_UNSIGNED_SHORT, NULL);

    // after completing the rendering, we tell the driver that the VAO vaoid
    // and the current shader program are no longer current
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    shdr_pgm_.UnUse();
}

void AnimationRenderer::Serialize(std::stringstream& data) {

    std::cout << "Serializing AnimationRenderer" << std::endl;

    std::string model;
    std::string shdr_pgm;
    int num_animations;

    data >> model
         >> shdr_pgm
         >> num_animations;

    for (int i = 0; i < num_animations; ++i)
    {
        std::string animation_name;
        data >> animation_name;
        AddAnimation(animation_name);
    }

    std::string current_animation_name;
    data >> current_animation_name;
    SetAnimation(current_animation_name);

    data >> play_animation_ >> has_finished_animating_;

    model_ = CORE->GetManager<ModelManager>()->GetModel(model);
    shdr_pgm_ = CORE->GetManager<ShaderManager>()->GetShdrpgm(shdr_pgm);
}