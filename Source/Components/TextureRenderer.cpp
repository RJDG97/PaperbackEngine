/**********************************************************************************
*\file         TextureRenderer.cpp
*\brief        Contains definition of functions and variables used for
*			   the TextureRenderer Component
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Components/TextureRenderer.h"
#include "Manager/ComponentManager.h"
#include "Components/Transform.h"
#include "Components/Scale.h"
#include "Engine/Core.h"
#include "Systems/GraphicsSystem.h"
#include <glm/gtc/type_ptr.hpp>

#define M_PI       3.14159265358979323846

TextureRenderer::TextureRenderer(): texture_name_{"Bug_Hill_2x1_0"}  {}

TextureRenderer::~TextureRenderer() {
    
    CORE->GetSystem<GraphicsSystem>()->RemoveTextureRendererComponent(Component::GetOwner()->GetID());
}

void TextureRenderer::Init() {
    
    CORE->GetSystem<GraphicsSystem>()->AddTextureRendererComponent(Component::GetOwner()->GetID(), this);

    InitTextures();
}

void TextureRenderer::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

    writer->StartObject();

    writer->Key("component");
    writer->String("TextureRenderer");

    writer->Key("texture");
    writer->String(texture_name_.c_str());

    writer->Key("layer");
    writer->String(std::to_string(layer_).c_str());

    writer->Key("order in layer");
    writer->String(std::to_string(order_in_layer_).c_str());

    writer->Key("alive");
    writer->String(std::to_string(alive_).c_str());

    writer->Key("opacity");
    writer->String(std::to_string(opacity_).c_str());

    writer->Key("tint");
    writer->String((std::to_string(tint_.x) + " " +
                    std::to_string(tint_.y) + " " +
                    std::to_string(tint_.z)).c_str());

    writer->EndObject();
}

void TextureRenderer::SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

    Serialize(writer);
}

void TextureRenderer::DeSerialize(std::stringstream& data) {

    data >> texture_name_
         >> layer_
         >> order_in_layer_
         >> alive_
         >> opacity_
         >> tint_.x >> tint_.y >> tint_.z;
}

void TextureRenderer::DeSerializeClone(std::stringstream& data) {

    //remove existing entry before proceeding
    CORE->GetSystem<GraphicsSystem>()->RemoveTextureRendererComponent(Component::GetOwner()->GetID());

    DeSerialize(data);

    //readd with new info
    CORE->GetSystem<GraphicsSystem>()->AddTextureRendererComponent(Component::GetOwner()->GetID(), this);
    InitTextures();
}

std::shared_ptr<Component> TextureRenderer::Clone() {

    M_DEBUG->WriteDebugMessage("Cloning TextureRenderer Component\n");

    std::shared_ptr<TextureRenderer> cloned = std::make_shared<TextureRenderer>();

    // IRenderer
    cloned->layer_ = layer_;
    cloned->order_in_layer_ = order_in_layer_;
    cloned->alive_ = alive_;
    cloned->opacity_ = opacity_;

    // SpriteRenderer
    cloned->tint_ = tint_;

    // TextureRenderer
    cloned->texture_name_ = texture_name_;
    cloned->texture_ = texture_;

    return cloned;
}

void TextureRenderer::InitTextures() {

    texture_ = CORE->GetManager<TextureManager>()->GetTexture(texture_name_);
    texture_handle_ = texture_->GetTilesetHandle();
    tex_vtx_ = *texture_->GetTexVtx();
}

std::string TextureRenderer::GetCurrentTextureName()
{
    return texture_name_;
}