/*                                                                      guard
----------------------------------------------------------------------------- */
#pragma once
#ifndef GRAPHICSSYSTEM_H
#define GRAPHICSSYSTEM_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Manager/ShaderManager.h"
#include "Systems/LightingSystem.h"
#include "Manager/TextureManager.h"
#include "Manager/AnimationManager.h"
#include "Systems/ISystem.h"
#include "Components/Renderer.h"
#include "Systems/Factory.h"
#include <unordered_map>
#include "Systems/WindowsSystem.h"
#include "Components/AnimationRenderer.h"
#include <windows.h>
#include <GL/glew.h> // for access to OpenGL API declarations
#include <GLFW/glfw3.h>

class GraphicsSystem : public ISystem {

    bool debug_;

    GLint window_width_;
    GLint window_height_;
    
    WindowsSystem* windows_system_;
    TextureManager* texture_manager_;
    AnimationManager* animation_manager_;
    ModelManager* model_manager_;
    ShaderManager* shader_manager_;

    //temp camera, will make it into a component next time!
    glm::mat3 view_xform_;
    glm::mat3 camwin_to_ndc_xform_;

    //render all game objects to texture
    GLuint frame_buffer_;
    GLuint render_buffer_;
    GLuint final_texture_;
    Model* final_model_;
    Shader* final_shader_;
    GLuint* lighting_texture_;

    void CameraInit();
    void CameraUpdate();

public:

    glm::vec2 cam_pos_;
    glm::vec2 cam_size_;

    glm::mat3 world_to_ndc_xform_;

    void Init();
    void Update(float frametime);
    void Draw() override;
    void DrawFinalTexture(Model* model, Shader* shader, GLuint* texture);
    void CleanUp();

    //returns the name of the system for debug use
    virtual std::string GetName();

    //function more akin to "What to do when message is received" for internal logic
    virtual void SendMessageD(Message* m);

    using RendererIt = std::unordered_map<EntityID, Renderer*>::iterator;
    void AddRendererComponent(EntityID id, Renderer* renderer);
    void RemoveRendererComponent(EntityID id);

    using AnimRendererIt = std::unordered_map<EntityID, AnimationRenderer*>::iterator;
    void AddAnimationRendererComponent(EntityID id, AnimationRenderer* animation_renderer);
    void RemoveAnimationRendererComponent(EntityID id);

    void UpdateObjectMatrix(Renderer* renderer, glm::mat3 world_to_ndc_xform);
    void UpdateAnimationFrame(AnimationRenderer* anim_renderer, float frametime);
    void DrawObject(Renderer* renderer);

    void ChangeTexture(Renderer* renderer, std::string texture_name);
    void ChangeModel(Renderer* renderer, std::string model_name);
    void ChangeShdrpgm(Renderer* renderer, std::string shdr_pgm_name);
    void FlipTextureX(Renderer* renderer);
    void FlipTextureY(Renderer* renderer);
    int GetLayer(Renderer* renderer);

    void AddAnimation(AnimationRenderer* anim_renderer, std::string animation_name);
    void SetAnimation(AnimationRenderer* anim_renderer, std::string animation_name);

    std::unordered_map<EntityID, Renderer*> renderer_arr_;
    std::unordered_map<EntityID, AnimationRenderer*> anim_renderer_arr_;

    using RenderOrderIt = std::multimap<int, Renderer*>::iterator;
    std::multimap<int, Renderer*> renderers_in_order_;
};

#endif