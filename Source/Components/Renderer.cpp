#include "Components/Renderer.h"
#include "Engine/Core.h"
#include "Systems/GraphicsSystem.h"
#include <glm/gtc/type_ptr.hpp>

#define M_PI       3.14159265358979323846

Renderer::Renderer()
{
    texture_ = *TEXTUREMANAGER->GetTexture(TextureName::Rock);
    scaling_ = glm::vec2{ 50.0f, 50.0f };
    orientation_ = glm::vec2{ 0.0f, 10.0f };
    position_ = glm::vec2{ 100.0f, 100.0f };
    model_ = MODELMANAGER->GetModel(ModelType::BoxModel);
    shdr_pgm_ = SHADERMANAGER->GetShdrpgm(ShaderType::TextureShader);
}

Renderer::~Renderer()
{
    CORE->GetSystem<GraphicsSystem>("GraphicsSystem")->RemoveRendererComponent(Component::GetOwner()->GetID());
}

void Renderer::Init()
{
    CORE->GetSystem<GraphicsSystem>("GraphicsSystem")->AddRendererComponent(Component::GetOwner()->GetID(), this);
}

void Renderer::PublishResults()
{
}

void Renderer::ChangeTexture(GLint tex_id)
{
    texture_ = *TEXTUREMANAGER->GetTexture(tex_id);
}

void Renderer::ChangeModel(GLint model_id)
{
    model_ = MODELMANAGER->GetModel(model_id);
}

void Renderer::ChangeShdrpgm(GLint shdr_pgm_id)
{
    shdr_pgm_ = SHADERMANAGER->GetShdrpgm(shdr_pgm_id);
}

void Renderer::Update(float frametime, glm::mat3 world_to_ndc_xform)
{
    glm::mat3 scale, rot, trans;

    scale = glm::mat3{ scaling_.x, 0.0f, 0.0f,
                       0.0f, scaling_.y, 0.0f,
                       0.0f, 0.0f, 1.0f };

    orientation_.x += orientation_.y * static_cast<float>(frametime);

    rot = glm::mat3{  glm::cos(orientation_.x * M_PI / 180), glm::sin(orientation_.x * M_PI / 180), 0.0f,
                     -glm::sin(orientation_.x * M_PI / 180), glm::cos(orientation_.x * M_PI / 180), 0.0f,
                      0.0f, 0.0f, 1.0f };


    trans = glm::mat3{ 1.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f,
                       position_.x, position_.y, 1.0f };

    mdl_to_ndc_xform_ = world_to_ndc_xform * trans * rot * scale;
}

void Renderer::Draw()
{
    shdr_pgm_.Use();
    glBindVertexArray(model_.vaoid_);

    glBindTexture(GL_TEXTURE_2D, texture_);
    glBindTextureUnit(0, texture_);
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
