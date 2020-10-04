#include "Components/AnimationRenderer.h"
#include "Components/Scale.h"
#include "Engine/Core.h"
#include "Systems/GraphicsSystem.h"
#include <glm/gtc/type_ptr.hpp>

#include "Components/Transform.h"

#define M_PI       3.14159265358979323846

AnimationRenderer::AnimationRenderer()
{
    AddAnimation("Player_Walk");
    SetAnimation("Player_Walk");
    play_animation_ = true;
    has_finished_animating = false;
    orientation_ = glm::vec2{ 0.0f, 10.0f };
    model_ = CORE->GetManager<ModelManager>()->GetModel("BoxModel");
    shdr_pgm_ = CORE->GetManager<ShaderManager>()->GetShdrpgm("TextureShader");
}

AnimationRenderer::~AnimationRenderer()
{
  
    CORE->GetSystem<GraphicsSystem>()->RemoveAnimationRendererComponent(Component::GetOwner()->GetID());
}

void AnimationRenderer::Init()
{
    /* Testing purposes
    Scale* e_scale = dynamic_cast<Scale*>(Component::GetOwner()->GetComponent(ComponentTypes::SCALE));
    assert(e_scale && "entity does not have a scale component");
    scaling_.x = e_scale->GetScale().x * 0.5f;
    scaling_.y = e_scale->GetScale().y * 0.5f;*/

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
    time_elapsed = 0.0f;
}

void AnimationRenderer::Update(float frametime, glm::mat3 world_to_ndc_xform)
{
    glm::mat3 scaling, rotation, translation;

    Vector2D scale = dynamic_cast<Scale*>(Component::GetOwner()->GetComponent(ComponentTypes::SCALE))->GetScale();

    scaling = glm::mat3{ scale.x, 0.0f, 0.0f,
                         0.0f, scale.y, 0.0f,
                         0.0f, 0.0f, 1.0f };

    //orientation_.x += orientation_.y * static_cast<float>(frametime);

    rotation = glm::mat3{  glm::cos(orientation_.x * M_PI / 180), glm::sin(orientation_.x * M_PI / 180), 0.0f,
                          -glm::sin(orientation_.x * M_PI / 180), glm::cos(orientation_.x * M_PI / 180), 0.0f,
                           0.0f, 0.0f, 1.0f };

    Transform* test = dynamic_cast<Transform*>(GetOwner()->GetComponent(ComponentTypes::TRANSFORM));

    assert(test);

    translation = glm::mat3{ 1.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f,
                       test->position_.x, test->position_.y, 1.0f };

    mdl_to_ndc_xform_ = world_to_ndc_xform * translation * rotation * scaling;

    if (play_animation_ == true)
    {
        time_elapsed += frametime;
        has_finished_animating = false;

        if (time_elapsed >= current_animation_->GetFrameDuration())
        {
            ++current_frame_;

            if (current_frame_ == current_animation_->GetLastFrame())
            {
                has_finished_animating = true;
                current_frame_ = current_animation_->GetFirstFrame();
            }

            time_elapsed = 0.0f;
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

    /*
    SetAnimation(AnimationName::Player_Attack);
    play_animation_ = true;
    has_finished_animating = false;
    scaling_ = glm::vec2{ 50.0f, 50.0f };
    orientation_ = glm::vec2{ 0.0f, 10.0f };
    position_ = glm::vec2{ 100.0f, 100.0f };
    model_ = MODELMANAGER->GetModel(ModelType::BoxModel);
    shdr_pgm_ = SHADERMANAGER->GetShdrpgm(ShaderType::TextureShader);
    */
}