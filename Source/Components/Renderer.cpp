#include "Components/Renderer.h"
#include "Components/Transform.h"
#include "Components/Scale.h"
#include "Engine/Core.h"
#include "Systems/GraphicsSystem.h"
#include <glm/gtc/type_ptr.hpp>

#define M_PI       3.14159265358979323846

Renderer::Renderer() {
    
}

Renderer::~Renderer() {
    
    CORE->GetSystem<GraphicsSystem>()->RemoveRendererComponent(Component::GetOwner()->GetID());
}

void Renderer::Init() {
    
    CORE->GetSystem<GraphicsSystem>()->AddRendererComponent(Component::GetOwner()->GetID(), this);
}

void Renderer::Serialize(std::stringstream& data) {
    
    std::string texture;
    std::string model;
    std::string shdr_pgm;

    data >> texture >> model >> shdr_pgm >> layer_;

    texture_ = *CORE->GetManager<TextureManager>()->GetTexture(texture);
    model_ = CORE->GetManager<ModelManager>()->GetModel(model);
    shdr_pgm_ = CORE->GetManager<ShaderManager>()->GetShdrpgm(shdr_pgm);
}
