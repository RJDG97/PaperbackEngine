/**********************************************************************************
*\file         GraphicsSystem.cpp
*\brief        Contains definition of functions and variables used for
*			   the Graphics System
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


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
#include "Components/Clickable.h"
#include "Components/Transform.h"
#include <glm/gtc/type_ptr.hpp>

// temporary function for checking
bool HasClickableAndActive(ComponentManager& mgr, EntityID id) {

    Clickable* clickable = mgr.GetComponent<Clickable>(id);

    if (clickable) {

        return (clickable->GetActive()) ? true : false;
    }
    //no clickable but active for rendering
    return true;
}

int GraphicsSystem::GetBatchSize()
{
    return batch_size_;
}

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

    // Clear colorbuffer with black color ...
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    windows_system_ = CORE->GetSystem<WindowsSystem>();
    camera_system_ = CORE->GetSystem<CameraSystem>();

    // Set up viewport
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
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
                          static_cast<GLsizei>(win_size_.x), static_cast<GLsizei>(win_size_.y));
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, render_buffer_);

    DEBUG_ASSERT(!(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE),
                 "Final framebuffer is not complete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //Set up all models and shaders
    model_manager_ = CORE->GetManager<ModelManager>();
    shader_manager_ = CORE->GetManager<ShaderManager>();
    font_manager_ = CORE->GetManager<FontManager>();
    texture_manager_ = CORE->GetManager<TextureManager>();
    animation_manager_ = CORE->GetManager<AnimationManager>();
    component_manager_ = CORE->GetManager<ComponentManager>();
    render_layers_ = CORE->GetManager<LayerManager>()->GetRenderLayers();

    batch_size_ = 500;

    graphic_models_["DebugModel"] = model_manager_->AddLinesModel("DebugModel", batch_size_);
    graphic_models_["BoxModel"] = model_manager_->AddTristripsModel(1, 1, "BoxModel");
    graphic_models_["TextModel"] = model_manager_->AddTextBatchModel(batch_size_, "TextModel");
    graphic_models_["UIModel"] = model_manager_->AddTristripsModel(1, 1, "UIModel");
    graphic_models_["BatchModel"] = model_manager_->AddTristripsBatchModel(batch_size_, "BatchModel");

    graphic_shaders_["WorldSpriteShader"] =
        shader_manager_->AddShdrpgm("Shaders/world_object.vert","Shaders/world_object.frag", "WorldSpriteShader");

    graphic_shaders_["TextShader"] =
        shader_manager_->AddShdrpgm("Shaders/text.vert", "Shaders/text.frag", "TextShader");

    graphic_shaders_["UISpriteShader"] =
        shader_manager_->AddShdrpgm("Shaders/ui_object.vert", "Shaders/ui_object.frag", "UISpriteShader");

    graphic_shaders_["FinalShader"] =
        shader_manager_->AddShdrpgm("Shaders/final.vert", "Shaders/final.frag", "FinalShader");

    graphic_shaders_["VignetteShader"] =
        shader_manager_->AddShdrpgm("Shaders/vignette.vert", "Shaders/vignette.frag", "VignetteShader");

    graphic_shaders_["DebugShader"] =
        shader_manager_->AddShdrpgm("Shaders/debug.vert", "Shaders/debug.frag", "DebugShader");

    lighting_texture_ = CORE->GetSystem<LightingSystem>()->GetLightingTexture();
    addition_texture_ = CORE->GetSystem<LightingSystem>()->GetAdditionTexture();

    //For UI and text
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

    if (camera_system_->GetMainCamera() == nullptr)
    {
        return;
    }

    for (AnimRendererIt it = anim_renderer_arr_->begin(); it != anim_renderer_arr_->end(); ++it) {

        if (!it->second->alive_) {

            continue;
        }

        if (debug_) {
            // Log id of entity and updated data members
            M_DEBUG->WriteDebugMessage("Updating entity: " + std::to_string(it->first) +
                " (Scale, Rotation, Translation matrix & Texture animation updated)\n");
        }
        
        UpdateAnimationFrame(it->second, frametime);
    }

    for (auto it = render_layers_->begin(); it != render_layers_->end(); ++it) {

        if (debug_) {
            // Log id of layer being updated
            M_DEBUG->WriteDebugMessage("Updating entity: " + std::to_string(it->first) +
                " (Scale, Rotation, Translation matrix & Texture animation updated)\n");
        }

        if (it->second.GetYSorted()) {
            
            UpdateRenderLayer(&it->second);
        }
    }
}

/*  _________________________________________________________________________ */
/*! GraphicsSystem::Draw

@param none

@return void

Clears the buffer and then draws a rectangular model in the viewport.
*/
void GraphicsSystem::Draw() {

    if (camera_system_->GetMainCamera() == nullptr)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        return;
    }

    if (debug_) { M_DEBUG->WriteDebugMessage("\nGraphics System Draw Debug Log:\n"); }

    world_to_ndc_xform_ = *camera_system_->GetMainCamera()->GetCameraWorldToNDCTransform();

    //temporary, will be removed after lighting layers is implemented!
    bool render_lights_now = true;

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_);
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto it = render_layers_->begin(); it != render_layers_->end(); ++it)
    {
        //temporary, will be removed after lighting layers is implemented!

        if ((it->second.GetLayerType() == UI_TEXT || it->second.GetLayerType() == UI_SPRITE) &&
            render_lights_now && lighting_enabled_) {

            glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
            DrawFinalTexture(lighting_texture_, 1.0f);
            glBlendFunc(GL_ONE, GL_ONE);
            DrawFinalTexture(addition_texture_, 0.6f);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            render_lights_now = false;
        }

        if (it->second.GetVisible()) {

            DrawLayer(it->second);
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    DrawFinalTexture(&final_texture_, 1.0f);

    if (debug_) { debug_ = !debug_; }
}

void GraphicsSystem::DrawFinalTexture(GLuint* texture, float opacity) {

    graphic_shaders_["FinalShader"]->Use();
    glBindVertexArray(graphic_models_["BoxModel"]->vaoid_);
    glBindTexture(GL_TEXTURE_2D, graphic_models_["BoxModel"]->vaoid_);
    glBindTextureUnit(0, *texture);
  
    graphic_shaders_["FinalShader"]->SetUniform("uTex2d", 0);
    graphic_shaders_["FinalShader"]->SetUniform("opacity", opacity);

    glDrawElements(GL_TRIANGLE_STRIP, graphic_models_["BoxModel"]->draw_cnt_, GL_UNSIGNED_SHORT, NULL);
    graphic_shaders_["FinalShader"]->UnUse();
    glBindVertexArray(0);
}

void GraphicsSystem::DrawVignette(float opacity) {

    graphic_shaders_["VignetteShader"]->Use();
    glBindVertexArray(graphic_models_["BoxModel"]->vaoid_);
    glBindTexture(GL_TEXTURE_2D, graphic_models_["BoxModel"]->vaoid_);

    graphic_shaders_["VignetteShader"]->SetUniform("center", { windows_system_->GetWinWidth() / 2,  windows_system_->GetWinHeight() / 2 });
    graphic_shaders_["VignetteShader"]->SetUniform("clear_size", vignette_size);
    graphic_shaders_["VignetteShader"]->SetUniform("max_size", max_vignette_size);
    graphic_shaders_["VignetteShader"]->SetUniform("opacity", opacity);

    glDrawElements(GL_TRIANGLE_STRIP, graphic_models_["BoxModel"]->draw_cnt_, GL_UNSIGNED_SHORT, NULL);
    graphic_shaders_["VignetteShader"]->UnUse();
    glBindVertexArray(0);
}

void GraphicsSystem::CleanUp() {

    glDeleteFramebuffers(1, &frame_buffer_);
    glDeleteRenderbuffers(1, &render_buffer_);
    glDeleteTextures(1, &final_texture_);
}

std::string GraphicsSystem::GetName() {

    return "GraphicsSystem";
}

void GraphicsSystem::SendMessageD(Message* m) {
    
    UNREFERENCED_PARAMETER(m);
    (void) m;
}

void GraphicsSystem::AddTextRendererComponent(EntityID id, TextRenderer* text_renderer)
{
    M_DEBUG->WriteDebugMessage("Adding Text Renderer Component to entity: " + std::to_string(id) + "\n");

    text_renderer_arr_->AddComponent(id, text_renderer);
    TextRenderer* it = text_renderer_arr_->GetComponent(id);

    DEBUG_ASSERT(render_layers_->find(it->GetLayer()) != render_layers_->end(), "Layer does not exist!");

    (*render_layers_)[it->GetLayer()].AddRenderer(text_renderer, it->GetOrderInLayer());
}

void GraphicsSystem::RemoveTextRendererComponent(EntityID id)
{
    TextRenderer* it = text_renderer_arr_->GetComponent(id);

    if (!it) {

        M_DEBUG->WriteDebugMessage("Text Renderer Component from entity had already been removed: " + std::to_string(id) + "\n");
        return;
    }

    M_DEBUG->WriteDebugMessage("Removing Text Renderer Component from entity: " + std::to_string(id) + "\n");

    RenderLayer* render_layer = &(*render_layers_)[it->GetLayer()];
    render_layer->RemoveRenderer(it);

    text_renderer_arr_->RemoveComponent(id);
}

void GraphicsSystem::AddTextureRendererComponent(EntityID id, TextureRenderer* texture_renderer) {

    M_DEBUG->WriteDebugMessage("Adding Texture Renderer Component to entity: " + std::to_string(id) + "\n");

    texture_renderer_arr_->AddComponent(id, texture_renderer);
    TextureRenderer* it = texture_renderer_arr_->GetComponent(id);

    DEBUG_ASSERT(render_layers_->find(it->GetLayer()) != render_layers_->end(), "Layer does not exist!");

    (*render_layers_)[it->GetLayer()].AddRenderer(texture_renderer, it->GetOrderInLayer());
}

void GraphicsSystem::RemoveTextureRendererComponent(EntityID id) {

    TextureRenderer* it = texture_renderer_arr_->GetComponent(id);

    if (!it) {

        M_DEBUG->WriteDebugMessage("Texture Renderer Component from entity had already been removed: " + std::to_string(id) + "\n");
        return;
    }

    M_DEBUG->WriteDebugMessage("Removing Texture Renderer Component from entity: " + std::to_string(id) + "\n");

    RenderLayer* render_layer = &(*render_layers_)[it->GetLayer()];
    render_layer->RemoveRenderer(it);

    texture_renderer_arr_->RemoveComponent(id);
}

void GraphicsSystem::AddAnimationRendererComponent(EntityID id, AnimationRenderer* animation_renderer) {

    M_DEBUG->WriteDebugMessage("Adding Animation Renderer Component to entity: " + std::to_string(id) + "\n");

    anim_renderer_arr_->AddComponent(id, animation_renderer);
    AnimationRenderer* it = anim_renderer_arr_->GetComponent(id);

    (*render_layers_)[it->GetLayer()].AddRenderer(animation_renderer, it->GetOrderInLayer());
}

void GraphicsSystem::RemoveAnimationRendererComponent(EntityID id) {

    AnimationRenderer* it = anim_renderer_arr_->GetComponent(id);

    if (!it) {

        M_DEBUG->WriteDebugMessage("Animation Renderer Component from entity had already been removed: " + std::to_string(id) + "\n");
        return;
    }

    M_DEBUG->WriteDebugMessage("Removing Animation Renderer Component from entity: "
        + std::to_string(id) + "\n");

    RenderLayer* render_layer = &(*render_layers_)[it->GetLayer()];
    render_layer->RemoveRenderer(it);

    anim_renderer_arr_->RemoveComponent(id);
}

void GraphicsSystem::UpdateAnimationFrame(AnimationRenderer* anim_renderer, float frametime) {

    if (anim_renderer->play_animation_ == true) {

        anim_renderer->time_elapsed_ += frametime * anim_renderer->animation_speed_;
        anim_renderer->has_finished_animating_ = false;

        if (anim_renderer->time_elapsed_ >= anim_renderer->current_animation_->GetFrameDuration()) {

            SetToNextFrame(anim_renderer);
            anim_renderer->total_time_elapsed_ += anim_renderer->time_elapsed_;

            if (IsLastFrame(anim_renderer)) {

                anim_renderer->has_finished_animating_ = true;
                anim_renderer->total_time_elapsed_ = 0.0f;

                if (anim_renderer->reversed_)
                {
                    SetToLastFrame(anim_renderer);
                }

                else
                {
                    SetToFirstFrame(anim_renderer);
                }
            }

            anim_renderer->time_elapsed_ = 0.0f;
        }
    }
}

void GraphicsSystem::UpdateRenderLayer(RenderLayer* render_layer) {

    std::multimap<float, IRenderer*>* current_order = render_layer->GetRenderers();
    std::multimap<float, IRenderer*> new_order;

    for (auto it = current_order->begin(); it != current_order->end(); ++it)
    {
        Transform* transform =
            component_manager_->GetComponent<Transform>(it->second->GetOwner()->GetID());
        
        new_order.insert({ transform->position_.y, it->second });
    }

    current_order->clear();
    current_order->merge(new_order);
}

void GraphicsSystem::BatchSpriteObject(SpriteRenderer* spr_renderer, bool ui) {

    Vector2D scale = component_manager_->GetComponent<Scale>(spr_renderer->GetOwner()->GetID())->GetScale();
    Transform* transform = component_manager_->GetComponent<Transform>(spr_renderer->GetOwner()->GetID());
    
    float orientation = static_cast<float>(transform->rotation_ * M_PI / 180);
    const float global_scale = CORE->GetGlobalScale();
    Vector2D pos = transform->position_ * global_scale;

    glm::vec2 scaling { scale.x, scale.y };
    glm::vec2 rotation { glm::cos(orientation), glm::sin(orientation) };
    glm::vec2 position{ pos.x, pos.y };

    if (ui)
    {
        position += 0.5f * win_size_;
    }

    if (texture_handles.find(spr_renderer->texture_handle_) == texture_handles.end()) {

        texture_handles[spr_renderer->texture_handle_] = static_cast<GLuint>(texture_handles.size());
    }

    GLuint tex_id = texture_handles[spr_renderer->texture_handle_];

    for (int i = 0; i < 4; ++i) {

        tex_vtx_sent.push_back(spr_renderer->tex_vtx_[i]);
        scaling_sent.push_back(scaling);
        rotation_sent.push_back(rotation);
        position_sent.push_back(position);
        texture_id_sent.push_back(static_cast<float>(tex_id));
        opacity_sent.push_back(spr_renderer->opacity_);
        color_sent.push_back(spr_renderer->tint_);
    }
}

void GraphicsSystem::BatchTextObject(TextRenderer* text_renderer, bool ui, GLuint vbo_hdl)
{
    Transform* transform = component_manager_->GetComponent<Transform>(text_renderer->GetOwner()->GetID());

    Vector2D pos;
    float scale = text_renderer->scale_;
    const float global_scale = CORE->GetGlobalScale();

    if (ui) {

        pos = transform->GetPosition() * global_scale + 0.5f * Vector2D{ win_size_.x, win_size_.y };
    }

    else {

        float cam_zoom = *camera_system_->GetMainCamera()->GetCameraZoom();
        glm::vec2 cam_pos = *camera_system_->GetMainCamera()->GetCameraPosition();

        glm::vec2 translation{ cam_pos * cam_zoom + 0.5f * win_size_ };
        pos = transform->GetPosition() * global_scale + Vector2D{ translation.x, translation.y };
        scale = text_renderer->scale_ * cam_zoom;
    }

    for (auto c = text_renderer->text_.begin(); c != text_renderer->text_.end(); c++) {

        Character ch = (*text_renderer->font_->GetCharacters())[*c];

        glm::ivec2 bearing = ch.GetBearing();
        glm::ivec2 size = ch.GetSize();

        float xpos = pos.x + bearing.x * scale;
        float ypos = pos.y - (size.y - bearing.y) * scale;

        float w = size.x * scale;
        float h = size.y * scale;

        if (texture_handles.find(ch.GetTexID()) == texture_handles.end()) {

            texture_handles[ch.GetTexID()] = static_cast<GLuint>(texture_handles.size());
        }

        GLuint tex_id = texture_handles[ch.GetTexID()];

        pos_vtx_sent.push_back({ xpos,     ypos + h });
        pos_vtx_sent.push_back({ xpos + w, ypos + h });
        pos_vtx_sent.push_back({ xpos,     ypos });
        pos_vtx_sent.push_back({ xpos + w, ypos });

        for (int i = 0; i < 4; ++i) {

            texture_id_sent.push_back(static_cast<float>(tex_id));
            color_sent.push_back(text_renderer->color_);
            opacity_sent.push_back(text_renderer->opacity_);
        }

        if (texture_handles.size() == 20)
        {
            DrawTextBatch(vbo_hdl);
        }

        pos.x += (ch.GetAdvance() >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)

    }
}

void GraphicsSystem::CheckDrawBatch(GLuint vbo_hdl, RenderLayer* render_layer, RenderOrderIt& it)
{
    if (tex_vtx_sent.size() == static_cast<size_t>(batch_size_) * 4 ||
        std::next(it, 1) == render_layer->GetRenderers()->end() ||
        texture_handles.size() == 20) {

        LayerType layer_type = render_layer->GetLayerType();

        switch (render_layer->GetLayerType()) {

            case UI_SPRITE: case WORLD_SPRITE: DrawSpriteBatch(vbo_hdl, layer_type); break;
            case UI_TEXT: case WORLD_TEXT: DrawTextBatch(vbo_hdl); break;
        }
    }
}

void GraphicsSystem::DrawSpriteBatch(GLuint vbo_hdl, LayerType layer_type)
{
    for (auto tex_it = texture_handles.begin(); tex_it != texture_handles.end(); ++tex_it) {

        glBindTextureUnit(tex_it->second, tex_it->first);
    }

    size_t offset = sizeof(glm::vec2) * 4 * batch_size_;

    glNamedBufferSubData(vbo_hdl, offset,
                         sizeof(glm::vec2) * tex_vtx_sent.size(),
                         tex_vtx_sent.data());

    offset += sizeof(glm::vec2) * 4 * batch_size_;

    glNamedBufferSubData(vbo_hdl, offset,
                         sizeof(glm::vec2) * scaling_sent.size(),
                         scaling_sent.data());

    offset += sizeof(glm::vec2) * 4 * batch_size_;

    glNamedBufferSubData(vbo_hdl, offset,
                         sizeof(glm::vec2) * rotation_sent.size(),
                         rotation_sent.data());

    offset += sizeof(glm::vec2) * 4 * batch_size_;

    glNamedBufferSubData(vbo_hdl, offset,
                         sizeof(glm::vec2) * position_sent.size(),
                         position_sent.data());

    offset += sizeof(glm::vec2) * 4 * batch_size_;

    glNamedBufferSubData(vbo_hdl, offset,
                         sizeof(float) * texture_id_sent.size(),
                         texture_id_sent.data());

    offset += sizeof(float) * 4 * batch_size_;

    glNamedBufferSubData(vbo_hdl, offset,
                         sizeof(float) * opacity_sent.size(),
                         opacity_sent.data());

    offset += sizeof(float) * 4 * batch_size_;

    glNamedBufferSubData(vbo_hdl, offset,
                         sizeof(glm::vec3) * color_sent.size(),
                         color_sent.data());

    int texture_samplers[20]{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };

    switch (layer_type)
    {
        case UI_SPRITE: {

            Shader* shader = graphic_shaders_["UISpriteShader"];
            shader->SetUniform("projection", projection);
            auto loc = glGetUniformLocation(shader->GetHandle(), "uTex2d");
            glUniform1iv(loc, 20, texture_samplers);
            break;
        }

        case WORLD_SPRITE: {

            Shader* shader = graphic_shaders_["WorldSpriteShader"];
            shader->SetUniform("world_to_ndc_xform", world_to_ndc_xform_);
            auto loc = glGetUniformLocation(shader->GetHandle(), "uTex2d");
            glUniform1iv(loc, 20, texture_samplers);
            break;
        }
    }

    glDrawElements(GL_TRIANGLE_STRIP, static_cast<GLsizei>(6 * tex_vtx_sent.size() / 4 - 2), GL_UNSIGNED_SHORT, NULL);

    tex_vtx_sent.clear();
    scaling_sent.clear();
    rotation_sent.clear();
    position_sent.clear();
    texture_id_sent.clear();
    texture_handles.clear();
    opacity_sent.clear();
    color_sent.clear();
}

void GraphicsSystem::DrawTextBatch(GLuint vbo_hdl)
{
    for (auto tex_it = texture_handles.begin(); tex_it != texture_handles.end(); ++tex_it) {

        glBindTextureUnit(tex_it->second, tex_it->first);
    }

    glNamedBufferSubData(vbo_hdl, 0,
                         sizeof(glm::vec2) * pos_vtx_sent.size(),
                         pos_vtx_sent.data());

    size_t offset = sizeof(glm::vec2) * 4 * batch_size_ * 2;

    glNamedBufferSubData(vbo_hdl, offset,
                         sizeof(float) * texture_id_sent.size(),
                         texture_id_sent.data());

    offset += sizeof(float) * 4 * batch_size_;

    glNamedBufferSubData(vbo_hdl, offset,
                         sizeof(glm::vec3) * color_sent.size(),
                         color_sent.data());

    offset += sizeof(glm::vec3) * 4 * batch_size_;

    glNamedBufferSubData(vbo_hdl, offset,
                         sizeof(float) * opacity_sent.size(),
                         opacity_sent.data());

    Shader* shader = graphic_shaders_["TextShader"];
    int texture_samplers[20]{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };

    auto loc = glGetUniformLocation(shader->GetHandle(), "uTex2d");
    glUniform1iv(loc, 20, texture_samplers);

    shader->SetUniform("projection", projection);
    glDrawElements(GL_TRIANGLE_STRIP, static_cast<GLsizei>(6 * pos_vtx_sent.size() / 4 - 2), GL_UNSIGNED_SHORT, NULL);

    pos_vtx_sent.clear();
    texture_id_sent.clear();
    color_sent.clear();
    texture_handles.clear();
    opacity_sent.clear();
}

void GraphicsSystem::DrawHealthbar(Shader* shader, Model* model, IRenderer* i_renderer)
{
    /*
    shader->SetUniform("uTex2d", 0);
    shader->SetUniform("projection", projection);

    Transform* xform = component_manager_->GetComponent<Transform>(i_renderer->GetOwner()->GetID());
    Scale* scale = component_manager_->GetComponent<Scale>(i_renderer->GetOwner()->GetID());

    Health* health = component_manager_->GetComponent<Health>(CORE->GetManager<EntityManager>()->GetPlayerEntities()[0]->GetID());

    Vector2D obj_pos_ = xform->position_ * CORE->GetGlobalScale() + 0.5f * Vector2D{ win_size_.x, win_size_.y };
    Vector2D obj_scale = scale->scale_ * 0.7f;

    std::vector<glm::vec2> gauge_vertices;
    std::vector<glm::vec2> water_vertices;

    gauge_vertices.push_back({ obj_pos_.x - obj_scale.x, obj_pos_.y - obj_scale.y });
    gauge_vertices.push_back({ obj_pos_.x + obj_scale.x, obj_pos_.y - obj_scale.y });
    gauge_vertices.push_back({ obj_pos_.x - obj_scale.x, obj_pos_.y + obj_scale.y });
    gauge_vertices.push_back({ obj_pos_.x + obj_scale.x, obj_pos_.y + obj_scale.y });

    obj_pos_.y -= 1.5f * obj_scale.y * (1.0f - health->GetCurrentHealth() / static_cast<float>(health->GetMaxHealth()));

    water_vertices.push_back({ obj_pos_.x - obj_scale.x, obj_pos_.y - obj_scale.y });
    water_vertices.push_back({ obj_pos_.x + obj_scale.x, obj_pos_.y - obj_scale.y });
    water_vertices.push_back({ obj_pos_.x - obj_scale.x, obj_pos_.y + obj_scale.y });
    water_vertices.push_back({ obj_pos_.x + obj_scale.x, obj_pos_.y + obj_scale.y });

    std::vector<glm::vec2> tex_vtx{ { 0.0f, 0.0f},
                                    { 1.0f, 0.0f},
                                    { 0.0f, 1.0f},
                                    { 1.0f, 1.0f}, };

    for (int i = 0; i < 4; ++i) {

        gauge_vertices.push_back(tex_vtx[i]);
        water_vertices.push_back(tex_vtx[i]);
    }
    
    glNamedBufferSubData(model->GetVBOHandle(), 0,
                         sizeof(glm::vec2) * gauge_vertices.size(), gauge_vertices.data());
    glBindTextureUnit(0, texture_manager_->GetTexture("WaterGauge_Leaves_0")->GetTilesetHandle());
    glDrawElements(GL_TRIANGLE_STRIP, model->draw_cnt_, GL_UNSIGNED_SHORT, NULL);
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    glNamedBufferSubData(model->GetVBOHandle(), 0,
                         sizeof(glm::vec2) * gauge_vertices.size(), gauge_vertices.data());
    glBindTextureUnit(0, texture_manager_->GetTexture("WaterGauge_Droplet_0")->GetTilesetHandle());
    glDrawElements(GL_TRIANGLE_STRIP, model->draw_cnt_, GL_UNSIGNED_SHORT, NULL);
    glDisable(GL_DEPTH_TEST);

    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glNamedBufferSubData(model->GetVBOHandle(), 0,
                         sizeof(glm::vec2) * water_vertices.size(), water_vertices.data());
    glBindTextureUnit(0, texture_manager_->GetTexture("WaterGauge_Water_0")->GetTilesetHandle());
    glDrawElements(GL_TRIANGLE_STRIP, model->draw_cnt_, GL_UNSIGNED_SHORT, NULL);
    glDisable(GL_STENCIL_TEST);

    glNamedBufferSubData(model->GetVBOHandle(), 0,
                         sizeof(glm::vec2) * gauge_vertices.size(), gauge_vertices.data());
    glBindTextureUnit(0, texture_manager_->GetTexture("WaterGauge_Shine_0")->GetTilesetHandle());
    glDrawElements(GL_TRIANGLE_STRIP, model->draw_cnt_, GL_UNSIGNED_SHORT, NULL);*/
}

void GraphicsSystem::ChangeLayer(IRenderer* i_renderer, int layer) {

    RenderLayer* new_layer = &(*render_layers_)[layer];
    RenderLayer* old_layer = &(*render_layers_)[i_renderer->GetLayer()];

    old_layer->RemoveRenderer(i_renderer);
    new_layer->AddRenderer(i_renderer, i_renderer->GetOrderInLayer());

    i_renderer->SetLayer(layer);
}

std::map<int, RenderLayer*> GraphicsSystem::GetAvailableLayers(IRenderer* i_renderer) {

    TextRenderer* text_renderer = dynamic_cast<TextRenderer*>(i_renderer);

    if (text_renderer == nullptr) {

        return GetLayersOfType(UI_SPRITE, WORLD_SPRITE);
    }

    return GetLayersOfType(UI_TEXT, WORLD_TEXT);
}

std::map<int, RenderLayer*> GraphicsSystem::GetLayersOfType(LayerType layer_type1, LayerType layer_type2) {

    std::map<int, RenderLayer*> render_layers;

    for (auto it = render_layers_->begin(); it != render_layers_->end(); ++it) {

        if (it->second.GetLayerType() == layer_type1) {

            render_layers[it->first] = &it->second;
        }

        else if (it->second.GetLayerType() == layer_type2) {

            render_layers[it->first] = &it->second;
        }
    }

    return render_layers;
}

void GraphicsSystem::FlipTextureX(SpriteRenderer* spr_renderer) {

    spr_renderer->x_mirror_ = !spr_renderer->x_mirror_;
    std::swap(spr_renderer->tex_vtx_[0], spr_renderer->tex_vtx_[2]);
    std::swap(spr_renderer->tex_vtx_[1], spr_renderer->tex_vtx_[3]);
}

void GraphicsSystem::FlipTextureY(SpriteRenderer* spr_renderer) {

    spr_renderer->y_mirror_ = !spr_renderer->y_mirror_;
    std::swap(spr_renderer->tex_vtx_[0], spr_renderer->tex_vtx_[1]);
    std::swap(spr_renderer->tex_vtx_[2], spr_renderer->tex_vtx_[3]);
}

void GraphicsSystem::CheckFlipTextureXY(SpriteRenderer* spr_renderer) {

    if (spr_renderer->x_mirror_) {

        std::swap(spr_renderer->tex_vtx_[0], spr_renderer->tex_vtx_[2]);
        std::swap(spr_renderer->tex_vtx_[1], spr_renderer->tex_vtx_[3]);
    }

    if (spr_renderer->y_mirror_) {

        std::swap(spr_renderer->tex_vtx_[0], spr_renderer->tex_vtx_[1]);
        std::swap(spr_renderer->tex_vtx_[2], spr_renderer->tex_vtx_[3]);
    }
}

void GraphicsSystem::ChangeTexture(TextureRenderer* texture_renderer, std::string texture_name) {

    if (texture_renderer->texture_name_ != texture_name) {
     
        texture_renderer->texture_ = (texture_manager_->GetTexture(texture_name));
        texture_renderer->texture_name_ = texture_name;
        texture_renderer->texture_handle_ = texture_renderer->texture_->GetTilesetHandle();
        texture_renderer->tex_vtx_ = *texture_renderer->texture_->GetTexVtx();
        CheckFlipTextureXY(texture_renderer);
    }
}

void GraphicsSystem::AddAnimation(AnimationRenderer* anim_renderer, std::string animation_name) {

    anim_renderer->obj_animations_[animation_name] = animation_manager_->GetAnimation(animation_name);
}

void GraphicsSystem::ChangeAnimation(AnimationRenderer* anim_renderer, std::string animation_name) {

    if (anim_renderer->current_animation_name_ == animation_name)
    {
        return;
    }

    SetAnimation(anim_renderer, animation_name);

    anim_renderer->time_elapsed_ = 0.0f;
    anim_renderer->total_time_elapsed_ = 0.0f;
    anim_renderer->play_animation_ = true;
}

void GraphicsSystem::SetAnimation(AnimationRenderer* anim_renderer, std::string animation_name) {
    
    auto it = anim_renderer->obj_animations_.find(animation_name);

    if (it == anim_renderer->obj_animations_.end()) {

        M_DEBUG->WriteDebugMessage("Tried to set to non-existant animation for entity: "
            + std::to_string(anim_renderer->GetOwner()->GetID()) + "\n");
        return;
    }

    anim_renderer->current_animation_ = &anim_renderer->obj_animations_[animation_name];
    anim_renderer->current_animation_name_ = animation_name;
    anim_renderer->time_elapsed_ = 0.0f;

    anim_renderer->texture_handle_ = anim_renderer->current_animation_->GetAnimationFramesHandle();

    if (anim_renderer->reversed_)
    {
        SetToLastFrame(anim_renderer);
    }

    else
    {
        SetToFirstFrame(anim_renderer);
    }
}

void GraphicsSystem::SetToNextFrame(AnimationRenderer* anim_renderer) {

    float offset = anim_renderer->current_animation_->GetOffsetX();

    if (anim_renderer->reversed_)
    {
        offset *= -1;
    }

    for (int i = 0; i < anim_renderer->tex_vtx_.size(); ++i) {

        anim_renderer->tex_vtx_[i].x += offset;
    }
}

void GraphicsSystem::SetToFirstFrame(AnimationRenderer* anim_renderer) {

    anim_renderer->tex_vtx_ = *(anim_renderer->current_animation_->GetTexVtx());
    CheckFlipTextureXY(anim_renderer);
}

void GraphicsSystem::SetToLastFrame(AnimationRenderer* anim_renderer) {

    anim_renderer->tex_vtx_ = *(anim_renderer->current_animation_->GetTexVtx());

    float offset = anim_renderer->current_animation_->GetOffsetX() *
                    (anim_renderer->current_animation_->GetNumFrames() - 1);
    
    for (size_t i = 0; i < anim_renderer->tex_vtx_.size(); ++i)
    {
        anim_renderer->tex_vtx_[i].x += offset;
    }

    CheckFlipTextureXY(anim_renderer);
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

void GraphicsSystem::DrawDebugLines(Points points, glm::vec4 color, float width)
{
    Shader* shader = graphic_shaders_["DebugShader"];
    Model* model = graphic_models_["DebugModel"];

    shader->Use();
    glBindVertexArray(model->vaoid_);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(width);

    shader->SetUniform("world_to_ndc_xform", *camera_system_->GetMainCamera()->GetCameraWorldToNDCTransform());
    shader->SetUniform("color", color);
    
    glNamedBufferSubData(model->GetVBOHandle(), 0, sizeof(glm::vec2) * points.size() * 2, points.data());
    glDrawElements(GL_LINES, static_cast<GLsizei>(points.size() * 2), GL_UNSIGNED_SHORT, NULL);
    glBindVertexArray(0);
    shader->UnUse();
}

void GraphicsSystem::SetVignetteSize(glm::vec2 size)
{
    vignette_size = size;
}

glm::vec2 GraphicsSystem::GetVignetteSize() {
   
    return vignette_size;
}

void GraphicsSystem::SetMaxVignetteSize(glm::vec2 size)
{
    max_vignette_size = size;
}

glm::vec2 GraphicsSystem::GetMaxVignetteSize() {

    return max_vignette_size;
}

void GraphicsSystem::EnableLighting(bool value)
{
    lighting_enabled_ = value;
}

std::vector<EntityID> GraphicsSystem::EntitiesWithThisTexture(GLuint handle)
{
    std::vector<EntityID> entities;

    for (auto it = texture_renderer_arr_->begin(); it != texture_renderer_arr_->end(); ++it)
    {
        if (it->second->texture_handle_ == handle)
        {
            entities.push_back(it->first);
        }
    }

    for (auto it = anim_renderer_arr_->begin(); it != anim_renderer_arr_->end(); ++it)
    {
        if (it->second->texture_handle_ == handle)
        {
            entities.push_back(it->first);
        }
    }
    
    return entities;
}

void GraphicsSystem::DrawLayer(RenderLayer& render_layer)
{
    LayerType layer_type = render_layer.GetLayerType();
    Model* model{};

    switch (layer_type)
    {
        case UI_SPRITE: {
            
            graphic_shaders_["UISpriteShader"]->Use();
            model = graphic_models_["BatchModel"];
            break;
        }

        case WORLD_SPRITE: {
            
            graphic_shaders_["WorldSpriteShader"]->Use();
            model = graphic_models_["BatchModel"];
            break;
        }

        case UI_TEXT: case WORLD_TEXT: {

            graphic_shaders_["TextShader"]->Use();
            model = graphic_models_["TextModel"];
            break;
        }

        case VIGNETTE: {

            DrawVignette(1.0f);
            return;
        }
    }

    if (model != nullptr)
    {
        glBindVertexArray(model->vaoid_);

        for (auto it = render_layer.GetRenderers()->begin();
            it != render_layer.GetRenderers()->end(); ++it) {

            if (!it->second->alive_) {

                CheckDrawBatch(model->vboid_, &render_layer, it);
                continue;
            }

            if (debug_) {

                M_DEBUG->WriteDebugMessage("Drawing entity: " + std::to_string(it->first) + "\n");
            }

            switch (layer_type)
            {
                case UI_SPRITE: {
                    
                    if (!HasClickableAndActive(*component_manager_, it->second->GetOwner()->GetID())) {
                        
                        CheckDrawBatch(model->vboid_, &render_layer, it);
                        continue;
                    }

                    //Temporary healthbar rendering
                    if (component_manager_->GetComponent<Name>(it->second->GetOwner()->GetID())->GetName() == "Watergauge")
                    {
                        DrawHealthbar(graphic_shaders_["UIShader"], graphic_models_["UIModel"], it->second);
                        CheckDrawBatch(model->vboid_, &render_layer, it);
                        continue;
                    }

                    BatchSpriteObject(static_cast<SpriteRenderer*>(it->second), true); break;
                }

                case WORLD_SPRITE: BatchSpriteObject(static_cast<SpriteRenderer*>(it->second), false); break;
                case UI_TEXT: BatchTextObject(static_cast<TextRenderer*>(it->second), true, model->vboid_); break;
                case WORLD_TEXT: BatchTextObject(static_cast<TextRenderer*>(it->second), false, model->vboid_); break;
            }

            CheckDrawBatch(model->vboid_, &render_layer, it);
        }
    }
}
