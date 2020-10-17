#include "Components/TextureRenderer.h"
#include "Components/Transform.h"
#include "Components/Scale.h"
#include "Engine/Core.h"
#include "Systems/GraphicsSystem.h"
#include <glm/gtc/type_ptr.hpp>

#define M_PI       3.14159265358979323846

TextureRenderer::TextureRenderer() {
    
}

TextureRenderer::~TextureRenderer() {
    
    CORE->GetSystem<GraphicsSystem>()->RemoveTextureRendererComponent(Component::GetOwner()->GetID());
}

void TextureRenderer::Init() {
    
    CORE->GetSystem<GraphicsSystem>()->AddTextureRendererComponent(Component::GetOwner()->GetID(), this);
    texture_handle_ = texture_.GetTilesetHandle();
    tex_vtx_initial_ = *texture_.GetTexVtx();
    tex_vtx_mirrored_ = std::vector<glm::vec2*>{ &tex_vtx_initial_[0],
                                                 &tex_vtx_initial_[1],
                                                 &tex_vtx_initial_[2],
                                                 &tex_vtx_initial_[3], };
    tex_vtx_sent_ = *texture_.GetTexVtx();
}

void TextureRenderer::Serialize(std::stringstream& data) {
    
    std::string texture;
    std::string model;
    std::string shdr_pgm;

    data >> texture >> model >> shdr_pgm >> layer_;

    texture_ = *CORE->GetManager<TextureManager>()->GetTexture(texture);
    model_ = CORE->GetManager<ModelManager>()->GetModel(model);
    shdr_pgm_ = CORE->GetManager<ShaderManager>()->GetShdrpgm(shdr_pgm);
}

std::shared_ptr<Component> TextureRenderer::Clone() {
    M_DEBUG->WriteDebugMessage("Cloning TextureRenderer Component\n");

    std::shared_ptr<TextureRenderer> cloned = std::make_shared<TextureRenderer>();

    // IRenderer
    cloned->model_ = model_; // check this
    cloned->shdr_pgm_ = shdr_pgm_; // check this
    cloned->layer_ = layer_;

    // TextureRenderer
    cloned->texture_ = texture_;

    return cloned;
}