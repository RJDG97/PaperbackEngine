#include "Renderer.h"
#include "Core.h"
#include "GraphicsSystem.h"
#include <glm/gtc/type_ptr.hpp>

#define M_PI       3.14159265358979323846

Renderer::Renderer()
{
    texture = *TEXTUREMANAGER->GetTexture(TextureName::Rock);
    scaling = glm::vec2{ 50.0f, 50.0f };
    orientation = glm::vec2{ 0.0f, 10.0f };
    position = glm::vec2{ 100.0f, 100.0f };
    model = MODELMANAGER->GetModel(ModelType::BoxModel);
    shdr_pgm = SHADERMANAGER->GetShdrpgm(ShaderType::TextureShader);
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

void Renderer::Update(float frametime, glm::mat3 world_to_ndc_xform)
{
    glm::mat3 scale, rot, trans;

    scale = glm::mat3{ scaling.x, 0.0f, 0.0f,
                       0.0f, scaling.y, 0.0f,
                       0.0f, 0.0f, 1.0f };

    orientation.x += orientation.y * static_cast<float>(frametime);

    rot = glm::mat3{ glm::cos(orientation.x * M_PI / 180), glm::sin(orientation.x * M_PI / 180), 0.0f,
                     -glm::sin(orientation.x * M_PI / 180), glm::cos(orientation.x * M_PI / 180), 0.0f,
                      0.0f, 0.0f, 1.0f };


    trans = glm::mat3{ 1.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f,
                       position.x, position.y, 1.0f };

    mdl_to_ndc_xform = world_to_ndc_xform * trans * rot * scale;
}

void Renderer::Draw()
{
    shdr_pgm.Use();
    glBindVertexArray(model.vaoid);

    glBindTexture(GL_TEXTURE_2D, texture);
    glBindTextureUnit(0, texture);
    glUseProgram(shdr_pgm.GetHandle());
    GLuint tex_loc = glGetUniformLocation(shdr_pgm.GetHandle(), "uTex2d");
    glUniform1i(tex_loc, 0);

    GLint uniform_var_loc1 =
        glGetUniformLocation(shdr_pgm.GetHandle(), "uModel_to_NDC");

    if (uniform_var_loc1 >= 0) {
        glUniformMatrix3fv(uniform_var_loc1, 1, GL_FALSE,
            glm::value_ptr(mdl_to_ndc_xform));
    }

    glDrawElements(GL_TRIANGLE_STRIP, model.draw_cnt, GL_UNSIGNED_SHORT, NULL);

    // after completing the rendering, we tell the driver that the VAO vaoid
    // and the current shader program are no longer current
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    shdr_pgm.UnUse();
}
