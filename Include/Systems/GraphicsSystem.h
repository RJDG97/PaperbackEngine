#pragma once
#ifndef GRAPHICSSYSTEM_H
#define GRAPHICSSYSTEM_H

#include "Manager/ShaderManager.h"
#include "Systems/LightingSystem.h"
#include "Manager/TextureManager.h"
#include "Manager/AnimationManager.h"
#include "Manager/ComponentManager.h"
#include "Systems/ISystem.h"
#include "Components/TextureRenderer.h"
#include "Systems/Factory.h"
#include <unordered_map>
#include "Systems/WindowsSystem.h"
#include "Systems/CameraSystem.h"
#include "Components/AnimationRenderer.h"
#include "Components/TextRenderer.h"
#include <windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GraphicsSystem : public ISystem {

    bool debug_;

    glm::vec2 win_size_;
    
    std::map<std::string, Shader*> graphic_shaders_;
    std::map<std::string, Model*> graphic_models_;

    std::shared_ptr<WindowsSystem> windows_system_;
    std::shared_ptr<CameraSystem> camera_system_;
    std::shared_ptr<TextureManager> texture_manager_;
    std::shared_ptr<AnimationManager> animation_manager_;
    std::shared_ptr<ModelManager> model_manager_;
    std::shared_ptr<ShaderManager> shader_manager_;
    std::shared_ptr<FontManager> font_manager_;
    std::shared_ptr<ComponentManager> component_manager_;

    //render all game objects to texture
    GLuint frame_buffer_;
    GLuint render_buffer_;
    GLuint final_texture_;
    GLuint* lighting_texture_;

    glm::mat4 projection;

    GLuint vao_batch_;
    GLuint vbo_batch_;
    GLuint ebo_batch_;

public:

/******************************************************************************/
/*!
    \fn Init()

    \brief Initializes the Graphics System
*/
/******************************************************************************/
    void Init();

/******************************************************************************/
/*!
    \fn Update()

    \brief Updates all renderer's Model to NDC transform and updates time
           elapsed for animation renderers
*/
/******************************************************************************/
    void Update(float frametime);

/******************************************************************************/
/*!
    \fn Draw()

    \brief Draws all objects with component that inherits from IRenderer
*/
/******************************************************************************/
    void Draw() override;

/******************************************************************************/
/*!
    \fn DrawFinalTexture(Model* model, Shader* shader, GLuint* texture)

    \brief Draws texture that covers the entire viewport (for post-processing)
*/
/******************************************************************************/
    void DrawFinalTexture(Model* model, Shader* shader, GLuint* texture);

/******************************************************************************/
/*!
    \fn CleanUp()

    \brief Cleans up the Graphics System
*/
/******************************************************************************/
    void CleanUp();

/******************************************************************************/
/*!
    \fn GetName()

    \brief Returns the name of the system for debug use
*/
/******************************************************************************/
    virtual std::string GetName();

/******************************************************************************/
/*!
    \fn SendMessageD(Message* m)

    \brief Function more akin to "What to do when message is received"
           for internal logic
*/
/******************************************************************************/
    virtual void SendMessageD(Message* m);

/******************************************************************************/
/*!
    \fn AddTextRendererComponent(EntityID id, TextRenderer* text_renderer)

    \brief Adds a TextRenderer component to the text renderer map
*/
/******************************************************************************/
void AddTextRendererComponent(EntityID id, TextRenderer* text_renderer);

/******************************************************************************/
/*!
    \fn RemoveTextRendererComponent(EntityID id)

    \brief Removes a TextRenderer component from the text renderer map
*/
/******************************************************************************/
void RemoveTextRendererComponent(EntityID id);

/******************************************************************************/
/*!
    \fn AddTextureRendererComponent(EntityID id, TextureRenderer* texture_renderer)

    \brief Adds a TextureRenderer component to the texture renderer map
*/
/******************************************************************************/
    void AddTextureRendererComponent(EntityID id, TextureRenderer* texture_renderer);

/******************************************************************************/
/*!
    \fn RemoveTextureRendererComponent(EntityID id)

    \brief Removes a TextureRenderer component from the texture renderer map
*/
/******************************************************************************/
    void RemoveTextureRendererComponent(EntityID id);

/******************************************************************************/
/*!
    \fn AddAnimationRendererComponent(EntityID id, AnimationRenderer* animation_renderer)

    \brief Adds an Animation Renderer component to the animation renderer map
*/
/******************************************************************************/
    void AddAnimationRendererComponent(EntityID id, AnimationRenderer* animation_renderer);

/******************************************************************************/
/*!
    \fn RemoveAnimationRendererComponent(EntityID id)

    \brief Removes an Animation Renderer component from the animation renderer map
*/
/******************************************************************************/
    void RemoveAnimationRendererComponent(EntityID id);

/******************************************************************************/
/*!
    \fn UpdateAnimationFrame(AnimationRenderer* anim_renderer, float frametime)

    \brief Updates time elapsed for animation renderers and changes the
           current frame
*/
/******************************************************************************/
    void UpdateAnimationFrame(AnimationRenderer* anim_renderer, float frametime);
    
/******************************************************************************/
/*!
    \fn DrawWorldObject(Shader* shader, Model* model, IWorldObjectRenderer* i_worldobj_renderer, glm::mat3 world_to_ndc_xform)

    \brief Draw objects that have a component that inherits from IObjectRenderer
*/
/******************************************************************************/
    void DrawWorldObject(Shader* shader, Model* model, IWorldObjectRenderer* i_worldobj_renderer, glm::mat3 world_to_ndc_xform);

/******************************************************************************/
/*!
    \fn DrawText(Shader* shader, Model* model,  TextRenderer* text_renderer, glm::vec2 cam_pos)

    \brief Draw text that have a TextRenderer component
*/
/******************************************************************************/
    void DrawTextObject(Shader* shader, Model* model, TextRenderer* text_renderer, glm::vec2 cam_pos);

/******************************************************************************/
/*!
    \fn FlipTextureX(IWorldObjectRenderer* i_worldobj_renderer)

    \brief Flips the texture renderered in the x axis
*/
/******************************************************************************/
    void FlipTextureX(IWorldObjectRenderer* i_worldobj_renderer);

/******************************************************************************/
/*!
\fn FlipTextureY(IWorldObjectRenderer* i_worldobj_renderer)

\brief Flips the texture renderered in the y axis
*/
/******************************************************************************/
    void FlipTextureY(IWorldObjectRenderer* i_worldobj_renderer);

/******************************************************************************/
/*!
\fn GetLayer(IWorldObjectRenderer* i_worldobj_renderer)

\brief Gets the layer that the texture will be renderered on
*/
/******************************************************************************/
    int GetLayer(IWorldObjectRenderer* i_worldobj_renderer);

/******************************************************************************/
/*!
\fn GetLayer(TextRenderer* text_renderer)

\brief Gets the layer that the text will be renderered on
*/
/******************************************************************************/
    int GetLayer(TextRenderer* text_renderer);

/******************************************************************************/
/*!
\fn ChangeTexture(TextureRenderer* renderer, std::string texture_name)

\brief Changes the texture rendered
*/
/******************************************************************************/
    void ChangeTexture(TextureRenderer* renderer, std::string texture_name);

/******************************************************************************/
/*!
\fn AddAnimation(AnimationRenderer* anim_renderer, std::string animation_name)

\brief Adds the animation specified into the animation renderer's
       animation map
*/
/******************************************************************************/
    void AddAnimation(AnimationRenderer* anim_renderer, std::string animation_name);

/******************************************************************************/
/*!
\fn SetAnimation(AnimationRenderer* anim_renderer, std::string animation_name)

\brief Sets the current animation to the specified animation in the animation
       renderer's animation map
*/
/******************************************************************************/
    void SetAnimation(AnimationRenderer* anim_renderer, std::string animation_name);

/******************************************************************************/
/*!
\fn SetToNextFrame(AnimationRenderer* anim_renderer)

\brief Set current frame to the next frame of the current animation
*/
/******************************************************************************/
    void SetToNextFrame(AnimationRenderer* anim_renderer);

/******************************************************************************/
/*!
\fn IsLastFrame(AnimationRenderer* anim_renderer)

\brief Checks if the current frame is the last frame of the animation
*/
/******************************************************************************/
    bool IsLastFrame(AnimationRenderer* anim_renderer);

/******************************************************************************/
/*!
\fn GetFrameBuffer()

\brief retrieves the frame buffer texture (for used in ImGui Viewport)
*/
/******************************************************************************/
    GLuint GetFramebuffer();


    //using TextRendererIt = std::unordered_map<EntityID, TextRenderer*>::iterator;
    //std::unordered_map<EntityID, TextRenderer*> text_renderer_arr_;
    using TextRendererType = CMap<TextRenderer>;
    using TextRendererIt = TextRendererType::MapTypeIt;
    TextRendererType* text_renderer_arr_;

    //using TextureRendererIt = std::unordered_map<EntityID, TextureRenderer*>::iterator;
    //std::unordered_map<EntityID, TextureRenderer*> texture_renderer_arr_;
    using TextureRendererType = CMap<TextureRenderer>;
    using TextureRendererIt = TextureRendererType::MapTypeIt;
    TextureRendererType* texture_renderer_arr_;

    //using AnimRendererIt = std::unordered_map<EntityID, AnimationRenderer*>::iterator;
    //std::unordered_map<EntityID, AnimationRenderer*> anim_renderer_arr_;
    using AnimationRendererType = CMap<AnimationRenderer>;
    using AnimRendererIt = AnimationRendererType::MapTypeIt;
    AnimationRendererType* anim_renderer_arr_;

    using WorldRenderOrderIt = std::multimap<int, IWorldObjectRenderer*>::iterator;
    using TextRenderOrderIt = std::multimap<int, TextRenderer*>::iterator;
    std::multimap<int, IWorldObjectRenderer*> worldobj_renderers_in_order_;
    std::multimap<int, TextRenderer*> worldtext_renderers_in_order_;
    std::multimap<int, IWorldObjectRenderer*> uirenderers_in_order_;
    std::multimap<int, TextRenderer*> uitext_renderers_in_order_;

};

#endif