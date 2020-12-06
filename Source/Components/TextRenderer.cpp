/**********************************************************************************
*\file         TextRenderer.cpp
*\brief        Contains definition of functions and variables used for
*			   the TextRenderer Component
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Components/TextRenderer.h"
#include "Manager/ComponentManager.h"
#include "Components/Transform.h"
#include "Components/Scale.h"
#include "Engine/Core.h"
#include "Systems/GraphicsSystem.h"

TextRenderer::TextRenderer() {

}

TextRenderer::~TextRenderer() {

    CORE->GetSystem<GraphicsSystem>()->RemoveTextRendererComponent(Component::GetOwner()->GetID());
}

void TextRenderer::Init() {

    CORE->GetSystem<GraphicsSystem>()->AddTextRendererComponent(Component::GetOwner()->GetID(), this);

    font_ = CORE->GetManager<FontManager>()->GetFont(font_name_);
}

void TextRenderer::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {
    
    writer->StartObject();

    writer->Key("component");
    writer->String("TextRenderer");

    writer->Key("font");
    writer->String((font_name_).c_str());

    writer->Key("sentence length");
    size_t a;
    writer->String(std::to_string(a = (std::count(text_.begin(), text_.end(), ' '))).c_str());

    writer->Key("text");
    writer->String((text_).c_str());

    writer->Key("color");
    writer->String((std::to_string(color_.x) + " " +
                    std::to_string(color_.y) + " " +
                    std::to_string(color_.z)).c_str());

    writer->Key("scale");
    writer->String(std::to_string(scale_).c_str());

    writer->Key("layer");
    writer->String(std::to_string(layer_).c_str());

    writer->Key("ui");
    writer->String(std::to_string(ui_).c_str());

    writer->Key("alive");
    writer->String(std::to_string(alive_).c_str());

    writer->EndObject();
}

void TextRenderer::DeSerialize(std::stringstream& data) {

    std::cout << "Serializing AnimationRenderer" << std::endl;

    int sentence_length;

    data >> font_name_
        >> sentence_length;

    for (int i = 0; i < sentence_length; ++i) {

        std::string temp;
        data >> temp;
        text_ += temp + " ";
    }

     data >> color_.x >> color_.y >> color_.z
          >> scale_
          >> layer_
          >> ui_
          >> alive_;
}


void TextRenderer::DeSerializeClone(std::stringstream& data)
{
    DeSerialize(data);
}

std::shared_ptr<Component> TextRenderer::Clone() {
    M_DEBUG->WriteDebugMessage("Cloning AnimationRenderer Component\n");

    std::shared_ptr<TextRenderer> cloned = std::make_shared<TextRenderer>();

    cloned->font_name_ = font_name_;
    cloned->font_ = font_;
    cloned->text_ = text_;
    cloned->color_ = color_;
    cloned->scale_ = scale_;
    cloned->layer_ = layer_;
    cloned->ui_ = ui_;
    cloned->alive_ = alive_;

    return cloned;
}

void TextRenderer::SetText(std::string text)
{
    text_ = text;
}
