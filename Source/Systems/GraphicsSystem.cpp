#define _USE_MATH_DEFINES

#include "Systems/GraphicsSystem.h"
#include "Engine/Core.h"
#include <FreeImage.h>
#include <cmath>
#include <string>
#include <set>
#include "Manager/ModelManager.h"
#include "Entity/ComponentTypes.h"
#include "Entity/ComponentCreator.h"
#include "Systems/Debug.h"
#include "Systems/Message.h"
#include "Systems/LightingSystem.h"
#include "Components/Scale.h"
#include "Components/Transform.h"
#include <glm/gtc/type_ptr.hpp>

/*  _________________________________________________________________________ */
/*! GraphicsSystem::Init

@param none

@return void

Initializes the OpenGL rendering pipeline, and then prints the OpenGL
Context information.
*/
void GraphicsSystem::Init() {

    ComponentManager* comp_mgr = &*CORE->GetManager<ComponentManager>();

    text_renderer_arr_ = comp_mgr->GetComponentArray<TextRenderer>();
    texture_renderer_arr_ = comp_mgr->GetComponentArray<TextureRenderer>();
    anim_renderer_arr_ = comp_mgr->GetComponentArray<AnimationRenderer>();

    // Clear colorbuffer with cyan color ...
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    windows_system_ = &*CORE->GetSystem<WindowsSystem>();
    camera_system_ = &*CORE->GetSystem<CameraSystem>();

    // Set up viewports
    win_size_.x = static_cast<float>(windows_system_->GetWinWidth());
    win_size_.y = static_cast<float>(windows_system_->GetWinHeight());
    glViewport(0, 0, static_cast<GLsizei>(win_size_.x), static_cast<GLsizei>(win_size_.y));

    // Set up frame buffer for rendering all objects to texture
    glGenFramebuffers(1, &frame_buffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_);

    glGenTextures(1, &final_texture_);
    glBindTexture(GL_TEXTURE_2D, final_texture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 static_cast<GLsizei>(win_size_.x), static_cast<GLsizei>(win_size_.y),
                 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, final_texture_, 0);

    glGenRenderbuffers(1, &render_buffer_);
    glBindRenderbuffer(GL_RENDERBUFFER, render_buffer_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, static_cast<GLsizei>(win_size_.x), static_cast<GLsizei>(win_size_.y));
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, render_buffer_);

    DEBUG_ASSERT(!(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE), "Final framebuffer is not complete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //Set up all models and shaders
    model_manager_ = &*CORE->GetManager<ModelManager>();
    shader_manager_ = &*CORE->GetManager<ShaderManager>();
    font_manager_ = &*CORE->GetManager<FontManager>();
    texture_manager_ = &*CORE->GetManager<TextureManager>();
    animation_manager_ = &*CORE->GetManager<AnimationManager>();

    graphic_models_["BoxModel"] = model_manager_->AddTristripsModel(1, 1, "BoxModel");
    graphic_models_["TileModel"] = model_manager_->AddTristripsModel(1, 1, "TileModel");
    graphic_models_["TextModel"] = model_manager_->AddTristripsModel(1, 1, "TextModel");

    graphic_shaders_["ObjectShader"] =
        shader_manager_->AddShdrpgm("Shaders/world_object.vert","Shaders/world_object.frag", "ObjectShader");

    graphic_shaders_["TextShader"] =
        shader_manager_->AddShdrpgm("Shaders/text.vert", "Shaders/text.frag", "TextShader");

    graphic_shaders_["FinalShader"] =
        shader_manager_->AddShdrpgm("Shaders/final.vert", "Shaders/final.frag", "FinalShader");

    font_manager_->LoadFont("comic_sans");

    lighting_texture_ = CORE->GetSystem<LightingSystem>()->GetLightingTexture();

    projection = glm::ortho(0.0f, win_size_.x, 0.0f, win_size_.y);

    M_DEBUG->WriteDebugMessage("Graphics System Init\n");
}

/*  _________________________________________________________________________ */
/*! GraphicsSystem::update

@param double delta_time

@return void

This also updates the size of the squares to be rendered in one of the tasks.
*/
void GraphicsSystem::Update(float frametime) {
    
    if (debug_) { M_DEBUG->WriteDebugMessage("\nGraphics System Update Debug Log:\n"); }
    
    glm::mat3 world_to_ndc_xform_ = camera_system_->world_to_ndc_xform_;

    //updates all the renderer components
    for (TextureRendererIt it = texture_renderer_arr_->begin(); it != texture_renderer_arr_->end(); ++it) {

        if (debug_) {
			// Log id of entity and it's updated components that are being updated
			M_DEBUG->WriteDebugMessage("Updating entity: " + std::to_string(it->first) + " (Scale, Rotation & Translation matrix updated)\n");
		}
        
        UpdateObjectMatrix(it->second, world_to_ndc_xform_);
    }

    for (AnimRendererIt it = anim_renderer_arr_->begin(); it != anim_renderer_arr_->end(); ++it) {

        if (debug_) {
            // Log id of entity and it's updated components that are being updated
            M_DEBUG->WriteDebugMessage("Updating entity: " + std::to_string(it->first) + " (Scale, Rotation, Translation matrix & Texture animation updated)\n");
        }
        
        UpdateObjectMatrix(it->second, world_to_ndc_xform_);
        UpdateAnimationFrame(it->second, frametime);
    }

}

/*  _________________________________________________________________________ */
/*! GraphicsSystem::Draw

@param none

@return void

Clears the buffer and then draws a rectangular model in the viewport.
*/
void GraphicsSystem::Draw() {

    if (debug_) { M_DEBUG->WriteDebugMessage("\nGraphics System Draw Debug Log:\n"); }

    glm::vec2 cam_pos_ = camera_system_->cam_pos_;

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_);
    glClear(GL_COLOR_BUFFER_BIT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    graphic_shaders_["ObjectShader"]->Use();
    glBindVertexArray(graphic_models_["TileModel"]->vaoid_);

    //draws all the world textures/animations
    for (WorldRenderOrderIt it = worldobj_renderers_in_order_.begin(); it != worldobj_renderers_in_order_.end(); ++it) {

        if (debug_) {
			// Log id of entity and its updated components that are being updated
			M_DEBUG->WriteDebugMessage("Drawing entity: " + std::to_string(it->first) + "\n");
		}

        DrawWorldObject(graphic_shaders_["ObjectShader"], graphic_models_["TileModel"], it->second);
    }

    glBindVertexArray(0);
    graphic_shaders_["ObjectShader"]->UnUse();

    graphic_shaders_["TextShader"]->Use();
    glBindVertexArray(graphic_models_["TextModel"]->vaoid_);

    //draws all the world text
    for (TextRenderOrderIt it = worldtext_renderers_in_order_.begin(); it != worldtext_renderers_in_order_.end(); ++it) {

        if (debug_) {
            // Log id of entity and its updated components that are being updated
            M_DEBUG->WriteDebugMessage("Drawing entity: " + std::to_string(it->first) + "\n");
        }

        DrawTextObject(graphic_shaders_["TextShader"], graphic_models_["TextModel"], it->second, cam_pos_);
    }

    glBindVertexArray(0);
    graphic_shaders_["TextShader"]->UnUse();

    glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    DrawFinalTexture(graphic_models_["BoxModel"], graphic_shaders_["FinalShader"], lighting_texture_);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    DrawFinalTexture(graphic_models_["BoxModel"], graphic_shaders_["FinalShader"], &final_texture_);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    graphic_shaders_["TextShader"]->Use();
    glBindVertexArray(graphic_models_["TextModel"]->vaoid_);

    for (TextRenderOrderIt it = uitext_renderers_in_order_.begin(); it != uitext_renderers_in_order_.end(); ++it) {

        if (debug_) {
            // Log id of entity and its updated components that are being updated
            M_DEBUG->WriteDebugMessage("Drawing entity: " + std::to_string(it->first) + "\n");
        }

        DrawTextObject(graphic_shaders_["TextShader"], graphic_models_["TextModel"], it->second, cam_pos_);
    }

    glBindVertexArray(0);
    graphic_shaders_["TextShader"]->UnUse();

    if (debug_) { debug_ = !debug_; }
}

void GraphicsSystem::DrawFinalTexture(Model* model, Shader* shader, GLuint* texture) {
    shader->Use();
    glBindVertexArray(model->vaoid_);

    glBindTexture(GL_TEXTURE_2D, *texture);
    glBindTextureUnit(0, *texture);
    glUseProgram(shader->GetHandle());
  
    shader->SetUniform("uTex2d", 0);

    glDrawElements(GL_TRIANGLE_STRIP, model->draw_cnt_, GL_UNSIGNED_SHORT, NULL);

    // after completing the rendering, we tell the driver that the VAO vaoid
    // and the current shader program are no longer current
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    shader->UnUse();
}

/*  _________________________________________________________________________ */
/*! GraphicsSystem::Cleanup

@param none

@return void

Returns allocated resources
*/
void GraphicsSystem::CleanUp() {

    glDeleteFramebuffers(1, &frame_buffer_);
    glDeleteRenderbuffers(1, &render_buffer_);
    glDeleteTextures(1, &final_texture_);
}

std::string GraphicsSystem::GetName() {

    return "GraphicsSystem";
}

void GraphicsSystem::SendMessageD(Message* m) {

    AnimRendererIt player_renderer;

    for (player_renderer = anim_renderer_arr_->begin(); player_renderer != anim_renderer_arr_->end(); ++player_renderer)
    {
        if (ENTITYNAME(player_renderer->second->GetOwner()) == "Player")
        {
            break;
        }
    }

    if (player_renderer == anim_renderer_arr_->end())
    {
        return;
    }

    switch(m->message_id_) {

        case MessageIDTypes::DEBUG_ALL: {
            debug_ = true;
            break;
        }
        
        case MessageIDTypes::CAM_UPDATE_POS: {
            //placeholder name for message, will be changed after engineproof
            MessagePhysics_Motion* msg = dynamic_cast<MessagePhysics_Motion*>(m);
            camera_system_->TempCameraMove(msg->new_vec_);
            break;
        }
        
        case MessageIDTypes::CHANGE_ANIMATION_1: {

            camera_system_->ToggleTargeted();
            SetAnimation(player_renderer->second, "Player_Walk");

            break;
        }
        
        case MessageIDTypes::CHANGE_ANIMATION_2: {

            SetAnimation(player_renderer->second, "Player_Idle");

            break;
        }

        case MessageIDTypes::FLIP_SPRITE_X: {

            FlipTextureX(dynamic_cast<IWorldObjectRenderer*>(player_renderer->second));
            camera_system_->TempCameraZoom(0.9f);
            break;
        }

        case MessageIDTypes::FLIP_SPRITE_Y: {

            FlipTextureY(dynamic_cast<IWorldObjectRenderer*>(player_renderer->second));
            camera_system_->TempCameraZoom(1.1f);
            break;
        }

        default: {

            break;
        }
    }
}

void GraphicsSystem::AddTextRendererComponent(EntityID id, TextRenderer* text_renderer)
{
    M_DEBUG->WriteDebugMessage("Adding Renderer Component to entity: " + std::to_string(id) + "\n");

    //text_renderer_arr_[id] = text_renderer;
    text_renderer_arr_->AddComponent(id, text_renderer);

    if (text_renderer->ui_text_)
    {
        uitext_renderers_in_order_.insert({ GetLayer(text_renderer), text_renderer });
    }

    else
    {
        worldtext_renderers_in_order_.insert({ GetLayer(text_renderer), text_renderer });
    }
}

void GraphicsSystem::RemoveTextRendererComponent(EntityID id)
{
    //TextRendererIt it = text_renderer_arr_.find(id);
    TextRenderer* it = text_renderer_arr_->GetComponent(id);

    int layer;

    if (it) {

        if (it->ui_text_)
        {
            TextRenderOrderIt orderit = uitext_renderers_in_order_.find(layer);

            if (orderit != uitext_renderers_in_order_.end()) {

                for (; orderit != uitext_renderers_in_order_.end() && (*orderit).first == layer; ++orderit) {

                    if ((*orderit).second->GetOwner()->GetID() == id) {

                        orderit = uitext_renderers_in_order_.erase(orderit);
                        break;
                    }
                }
            }
        }

        else
        {
            TextRenderOrderIt orderit = worldtext_renderers_in_order_.find(layer);

            if (orderit != worldtext_renderers_in_order_.end()) {

                for (; orderit != worldtext_renderers_in_order_.end() && (*orderit).first == layer; ++orderit) {

                    if ((*orderit).second->GetOwner()->GetID() == id) {

                        orderit = worldtext_renderers_in_order_.erase(orderit);
                        break;
                    }
                }
            }
        }

        M_DEBUG->WriteDebugMessage("Removing Renderer Component from entity: " + std::to_string(id) + "\n");
        layer = GetLayer(it);
        text_renderer_arr_->RemoveComponent(id);
    }
}

void GraphicsSystem::AddTextureRendererComponent(EntityID id, TextureRenderer* texture_renderer) {

    M_DEBUG->WriteDebugMessage("Adding Renderer Component to entity: " + std::to_string(id) + "\n");

    //texture_renderer_arr_[id] = texture_renderer;
    texture_renderer_arr_->AddComponent(id, texture_renderer);
    worldobj_renderers_in_order_.insert({GetLayer(texture_renderer), texture_renderer });
}

void GraphicsSystem::RemoveTextureRendererComponent(EntityID id) {

    //TextureRendererIt it = texture_renderer_arr_.find(id);
    TextureRenderer* it = texture_renderer_arr_->GetComponent(id);
    int layer;

    if (it) {

        M_DEBUG->WriteDebugMessage("Removing Renderer Component from entity: " + std::to_string(id) + "\n");
        layer = GetLayer(it);
        //texture_renderer_arr_.erase(it);
        texture_renderer_arr_->RemoveComponent(id);
    }

    WorldRenderOrderIt orderit = worldobj_renderers_in_order_.find(layer);

    if (orderit != worldobj_renderers_in_order_.end()) {

        for ( ; orderit != worldobj_renderers_in_order_.end() && (*orderit).first == layer ; ++orderit) {

            if ((*orderit).second->GetOwner()->GetID() == id) {

                orderit = worldobj_renderers_in_order_.erase(orderit);
                break;
            }
        }
    }
}

void GraphicsSystem::AddAnimationRendererComponent(EntityID id, AnimationRenderer* animation_renderer) {

    M_DEBUG->WriteDebugMessage("Adding Animation Renderer Component to entity: " + std::to_string(id) + "\n");

    //anim_renderer_arr_[id] = animation_renderer;
    anim_renderer_arr_->AddComponent(id, animation_renderer);
    worldobj_renderers_in_order_.insert({GetLayer(animation_renderer), animation_renderer});
}

void GraphicsSystem::RemoveAnimationRendererComponent(EntityID id) {

    //AnimRendererIt it = anim_renderer_arr_.find(id);
    AnimationRenderer* it = anim_renderer_arr_->GetComponent(id);
    int layer;

    if (it) {

        M_DEBUG->WriteDebugMessage("Removing Animation Renderer Component from entity: " + std::to_string(id) + "\n");
        layer = GetLayer(it);
        //anim_renderer_arr_.erase(it);
        anim_renderer_arr_->RemoveComponent(id);
    }

    WorldRenderOrderIt orderit = worldobj_renderers_in_order_.find(layer);

    if (orderit != worldobj_renderers_in_order_.end()) {

        for (; orderit != worldobj_renderers_in_order_.end() && (*orderit).first == layer; ++orderit) {

            if ((*orderit).second->GetOwner()->GetID() == id) {

                orderit = worldobj_renderers_in_order_.erase(orderit);
                break;
            }
        }
    }
}

void GraphicsSystem::UpdateObjectMatrix(IWorldObjectRenderer* i_worldobj_renderer, glm::mat3 world_to_ndc_xform) {

    glm::mat3 scaling, rotation, translation;

    Vector2D scale = std::dynamic_pointer_cast<Scale>(i_worldobj_renderer->GetOwner()->GetComponent(ComponentTypes::SCALE))->GetScale();

    scaling = glm::mat3{ scale.x, 0.0f, 0.0f,
                         0.0f, scale.y, 0.0f,
                         0.0f, 0.0f, 1.0f };

    std::shared_ptr<Transform> transform = 
        std::dynamic_pointer_cast<Transform>(i_worldobj_renderer->GetOwner()->GetComponent(ComponentTypes::TRANSFORM));
    //assert(transform);
    DEBUG_ASSERT(transform, "Entity does not have a Transform component");

    float orientation = transform->rotation_;
    Vector2D position = transform->position_;

    rotation = glm::mat3{ glm::cos(orientation * M_PI / 180), glm::sin(orientation * M_PI / 180), 0.0f,
                          -glm::sin(orientation * M_PI / 180), glm::cos(orientation * M_PI / 180), 0.0f,
                           0.0f, 0.0f, 1.0f };

    translation = glm::mat3{ 1.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f,
                       position.x, position.y, 1.0f };

    i_worldobj_renderer->mdl_to_ndc_xform_ = world_to_ndc_xform * translation * rotation * scaling;
}

void GraphicsSystem::UpdateAnimationFrame(AnimationRenderer* anim_renderer, float frametime) {

    if (anim_renderer->play_animation_ == true) {

        anim_renderer->time_elapsed_ += frametime;
        anim_renderer->has_finished_animating_ = false;

        if (anim_renderer->time_elapsed_ >= anim_renderer->current_animation_->GetFrameDuration()) {

            SetToNextFrame(anim_renderer);
            anim_renderer->total_time_elapsed_ += anim_renderer->time_elapsed_;

            if (IsLastFrame(anim_renderer)) {

                anim_renderer->has_finished_animating_ = true;
                anim_renderer->total_time_elapsed_ = 0.0f;

                for (int i = 0; i < anim_renderer->tex_vtx_sent_.size(); ++i) {

                    anim_renderer->tex_vtx_sent_[i] = *anim_renderer->tex_vtx_mirrored_[i];
                }
            }

            anim_renderer->time_elapsed_ = 0.0f;
        }
    }
}

void GraphicsSystem::DrawWorldObject(Shader* shader, Model* model, IWorldObjectRenderer* i_worldobj_renderer) {
    
    glBindTexture(GL_TEXTURE_2D, *(i_worldobj_renderer->texture_handle_));
    glBindTextureUnit(0, *(i_worldobj_renderer->texture_handle_));
    glUseProgram(shader->GetHandle());

    shader->SetUniform("uTex2d", 0);
    shader->SetUniform("uModel_to_NDC", i_worldobj_renderer->mdl_to_ndc_xform_);

    glNamedBufferSubData(model->GetVBOHandle(), model->GetVBOOffset(),
                         sizeof(i_worldobj_renderer->tex_vtx_sent_), i_worldobj_renderer->tex_vtx_sent_.data());
    glDrawElements(GL_TRIANGLE_STRIP, model->draw_cnt_, GL_UNSIGNED_SHORT, NULL);

    // after completing the rendering, we tell the driver that the VAO vaoid
    // and the current shader program are no longer current

    glBindTexture(GL_TEXTURE_2D, 0);
}

void GraphicsSystem::DrawTextObject(Shader* shader, Model* model, TextRenderer* text_renderer, glm::vec2 cam_pos) {

    shader->SetUniform("uTex2d", 0);
    shader->SetUniform("projection", projection);
    shader->SetUniform("text_color", text_renderer->color_);

    Vector2D obj_pos_ = std::dynamic_pointer_cast<Transform>(
        text_renderer->GetOwner()->GetComponent(ComponentTypes::TRANSFORM))->position_;

    Vector2D pos;
    float scale;

    if (text_renderer->ui_text_)
    {
        pos = obj_pos_;
        scale = text_renderer->scale_;
    }

    else
    {
        glm::vec2 translation{ cam_pos * camera_system_->cam_zoom_ + 0.5f * win_size_ };
        pos = obj_pos_ + Vector2D{ translation.x, translation.y };
        scale = text_renderer->scale_ * camera_system_->cam_zoom_;
    }

    std::string::const_iterator c;
    for (c = text_renderer->text_.begin(); c != text_renderer->text_.end(); c++)
    {
        Character ch = (*text_renderer->font_->GetCharacters())[*c];

        glm::ivec2 bearing = ch.GetBearing();
        glm::ivec2 size = ch.GetSize();

        float xpos = pos.x + bearing.x * scale;
        float ypos = pos.y - (size.y - bearing.y) * scale;

        float w = size.x * scale;
        float h = size.y * scale;
        
        std::vector<glm::vec2> vertices;

        vertices.push_back({ xpos,     ypos + h });
        vertices.push_back({ xpos + w, ypos + h });
        vertices.push_back({ xpos,     ypos });
        vertices.push_back({ xpos + w, ypos });

        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.GetTexID());

        glNamedBufferSubData(model->GetVBOHandle(), 0,
            sizeof(vertices), vertices.data());

        glDrawElements(GL_TRIANGLE_STRIP, model->draw_cnt_, GL_UNSIGNED_SHORT, NULL);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        pos.x += (ch.GetAdvance() >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GraphicsSystem::FlipTextureX(IWorldObjectRenderer* i_worldobj_renderer) {

    i_worldobj_renderer->x_mirror_ = !i_worldobj_renderer->x_mirror_;

    std::swap(i_worldobj_renderer->tex_vtx_mirrored_[0], i_worldobj_renderer->tex_vtx_mirrored_[2]);
    std::swap(i_worldobj_renderer->tex_vtx_mirrored_[1], i_worldobj_renderer->tex_vtx_mirrored_[3]);

    i_worldobj_renderer->tex_vtx_sent_.clear();

    for (int i = 0; i < i_worldobj_renderer->tex_vtx_mirrored_.size(); ++i) {

        i_worldobj_renderer->tex_vtx_sent_.push_back(*i_worldobj_renderer->tex_vtx_mirrored_[i]);
    }
}

void GraphicsSystem::FlipTextureY(IWorldObjectRenderer* i_worldobj_renderer) {

    i_worldobj_renderer->y_mirror_ = !i_worldobj_renderer->y_mirror_;

    std::swap(i_worldobj_renderer->tex_vtx_mirrored_[0], i_worldobj_renderer->tex_vtx_mirrored_[1]);
    std::swap(i_worldobj_renderer->tex_vtx_mirrored_[2], i_worldobj_renderer->tex_vtx_mirrored_[3]);

    i_worldobj_renderer->tex_vtx_sent_.clear();

    for (int i = 0; i < i_worldobj_renderer->tex_vtx_mirrored_.size(); ++i) {

        i_worldobj_renderer->tex_vtx_sent_.push_back(*i_worldobj_renderer->tex_vtx_mirrored_[i]);
    }
}

int GraphicsSystem::GetLayer(IWorldObjectRenderer* i_worldobj_renderer) {

    return i_worldobj_renderer->layer_;
}

int GraphicsSystem::GetLayer(TextRenderer* text_renderer)
{
    return text_renderer->layer_;
}

void GraphicsSystem::ChangeTexture(TextureRenderer* renderer, std::string texture_name) {

    renderer->texture_ = *(texture_manager_->GetTexture(texture_name));
}

void GraphicsSystem::AddAnimation(AnimationRenderer* anim_renderer, std::string animation_name) {

    anim_renderer->obj_animations_[animation_name] = animation_manager_->GetAnimation(animation_name);
}

void GraphicsSystem::SetAnimation(AnimationRenderer* anim_renderer, std::string animation_name) {

    anim_renderer->current_animation_ = &anim_renderer->obj_animations_[animation_name];
    anim_renderer->time_elapsed_ = 0.0f;

    anim_renderer->texture_handle_ = anim_renderer->current_animation_->GetAnimationFramesHandle();
    anim_renderer->tex_vtx_initial_ = *anim_renderer->current_animation_->GetTexVtx();

    anim_renderer->tex_vtx_sent_.clear();

    for (int i = 0; i < anim_renderer->tex_vtx_mirrored_.size(); ++i) {

        anim_renderer->tex_vtx_sent_.push_back(*anim_renderer->tex_vtx_mirrored_[i]);
    }
}

void GraphicsSystem::SetToNextFrame(AnimationRenderer* anim_renderer) {

    for (int i = 0; i < anim_renderer->tex_vtx_sent_.size(); ++i) {

        anim_renderer->tex_vtx_sent_[i].x +=
            anim_renderer->current_animation_->GetOffsetX() * ( 1 - 2 * anim_renderer->y_mirror_);
    }
}

bool GraphicsSystem::IsLastFrame(AnimationRenderer* anim_renderer) {

    return anim_renderer->total_time_elapsed_ >=
                anim_renderer->current_animation_->GetFrameDuration() *
                anim_renderer->current_animation_->GetNumFrames();
}

GLuint GraphicsSystem::GetFramebuffer()
{
    return final_texture_;
}
