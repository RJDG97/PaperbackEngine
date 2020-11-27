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

    class RenderLayer {

        std::string name_;
        bool y_sorted;
        std::multimap<int, IRenderer*> renderers_;

    };

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
    GLuint* addition_texture_;

    //for UI
    glm::mat4 projection;

    //for batching
    int batch_size_;
    std::vector<glm::vec2> tex_vtx_sent;
    std::vector<glm::vec2> scaling_sent;
    std::vector<glm::vec2> rotation_sent;
    std::vector<glm::vec2> position_sent;
    std::vector<float> texture_id_sent;
    std::map<GLuint, GLuint> texture_handles;

    glm::vec2 vignette_size;

    using Points = std::vector<std::pair<glm::vec2, glm::vec2>>;

public:

/******************************************************************************/
/*!
    \fn GetBatchSize()

    \brief Returns batch size
*/
/******************************************************************************/
    int GetBatchSize();

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
    \fn DrawFinalTexture(GLuint* texture, float opacity)

    \brief Draws texture that covers the entire viewport (for post-processing)
*/
/******************************************************************************/
    void DrawFinalTexture(GLuint* texture, float opacity);

/******************************************************************************/
/*!
    \fn DrawFinalTextureWithVignette(GLuint* texture, float opacity)

    \brief Draws texture that covers the entire viewport (for post-processing)
*/
/******************************************************************************/
    void DrawFinalTextureWithVignette(GLuint* texture, float opacity);

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
    \fn BatchWorldObject(IRenderer* i_renderer)

    \brief Inserts data of objects, that have a component that inherits from
           IRenderer, into batch.
*/
/******************************************************************************/
    void BatchWorldObject(IRenderer* i_renderer);

/******************************************************************************/
/*!
    \fn DrawBatch(GLuint vbo_hdl, glm::mat3 world_to_ndc_xform)

    \brief Draw all objects in the batch
*/
/******************************************************************************/
    void DrawBatch(GLuint vbo_hdl, glm::mat3 world_to_ndc_xform);

/******************************************************************************/
/*!
    \fn DrawTextObject(Shader* shader, Model* model,
                       TextRenderer* text_renderer)

    \brief Draw objects that have a TextRenderer component
*/
/******************************************************************************/
    void DrawTextObject(Shader* shader, Model* model,
                        TextRenderer* text_renderer);

/******************************************************************************/
/*!
    \fn DrawUIObject(Shader* shader, Model* model, IRenderer* i_renderer)

    \brief Draw UI objects that have a IRenderer component
*/
/******************************************************************************/
    void DrawUIObject(Shader* shader, Model* model, IRenderer* i_renderer);

/******************************************************************************/
/*!
    \fn DrawHealthbar(Shader* shader, Model* model, IRenderer* i_renderer)

    \brief Draws the healthbar
*/
/******************************************************************************/
    void DrawHealthbar(Shader* shader, Model* model, IRenderer* i_renderer);

/******************************************************************************/
/*!
    \fn FlipTextureX(IRenderer* i_renderer)

    \brief Flips the texture renderered in the x axis
*/
/******************************************************************************/
    void FlipTextureX(IRenderer* i_worldobj_renderer);

/******************************************************************************/
/*!
\fn FlipTextureY(IRenderer* i_renderer)

\brief Flips the texture renderered in the y axis
*/
/******************************************************************************/
    void FlipTextureY(IRenderer* i_renderer);

/******************************************************************************/
/*!
\fn GetLayer(IRenderer* i_renderer)

\brief Gets the layer that the texture will be renderered on
*/
/******************************************************************************/
    int GetLayer(IRenderer* i_renderer);

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
    void AddAnimation(AnimationRenderer* anim_renderer,
                      std::string animation_name);

/******************************************************************************/
/*!
\fn SetAnimation(AnimationRenderer* anim_renderer, std::string animation_name)

\brief Sets the current animation to the specified animation in the animation
       renderer's animation map
*/
/******************************************************************************/
    void SetAnimation(AnimationRenderer* anim_renderer,
                      std::string animation_name);

/******************************************************************************/
/*!
\fn ChangeAnimation(AnimationRenderer* anim_renderer, std::string animation_name)

\brief Changes the current animation to the specified animation in the animation
       renderer's animation map
*/
/******************************************************************************/
    void ChangeAnimation(AnimationRenderer* anim_renderer,
                      std::string animation_name);

/******************************************************************************/
/*!
\fn SetToNextFrame(AnimationRenderer* anim_renderer)

\brief Set current frame to the next frame of the current animation
*/
/******************************************************************************/
    void SetToNextFrame(AnimationRenderer* anim_renderer);

/******************************************************************************/
/*!
\fn SetToFirstFrame(AnimationRenderer* anim_renderer)

\brief Set current frame to the first frame of the current animation
*/
/******************************************************************************/
    void SetToFirstFrame(AnimationRenderer* anim_renderer);

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

/******************************************************************************/
/*!
    \fn SetVignetteSize(glm::vec2 size)

    \brief Set vignette's size
*/
/******************************************************************************/
    void SetVignetteSize(glm::vec2 size);

    glm::vec2 GetVignetteSize();
    void DrawDebugLines(Points points, glm::vec4 color, float width);

    using TextRendererType = CMap<TextRenderer>;
    using TextRendererIt = TextRendererType::MapTypeIt;
    TextRendererType* text_renderer_arr_;

    using TextureRendererType = CMap<TextureRenderer>;
    using TextureRendererIt = TextureRendererType::MapTypeIt;
    TextureRendererType* texture_renderer_arr_;

    using AnimationRendererType = CMap<AnimationRenderer>;
    using AnimRendererIt = AnimationRendererType::MapTypeIt;
    AnimationRendererType* anim_renderer_arr_;

    using IRenderOrderIt = std::multimap<int, IRenderer*>::iterator;
    using TextRenderOrderIt = std::multimap<int, TextRenderer*>::iterator;
    std::multimap<int, IRenderer*> worldobj_renderers_in_order_;
    std::multimap<int, TextRenderer*> worldtext_renderers_in_order_;
    std::multimap<int, IRenderer*> uirenderers_in_order_;
    std::multimap<int, TextRenderer*> uitext_renderers_in_order_;

    std::map<int, RenderLayer> render_layers_;
};

#endif