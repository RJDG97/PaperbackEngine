/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GRAPHICSSYSTEM_H
#define GRAPHICSSYSTEM_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <GL/glew.h> // for access to OpenGL API declarations
#include <GLFW/glfw3.h>
#include "ShaderManager.h"
#include "LightingSystem.h"
#include "TextureManager.h"
#include "AnimationManager.h"
#include "ISystem.h"
#include "Renderer.h"
#include "Factory.h"
#include <unordered_map>
#include "WindowsSystem.h"
#include "AnimationRenderer.h"

class GraphicsSystem : public ISystem
{
    GLint window_width_;
    GLint window_height_;
    
    WindowsSystem* windows_system_;
    //LightingSystem* lighting_system;

    //temp camera, will make it into a gameobject next time!
    glm::vec2 cam_pos_;
    glm::vec2 cam_size_;
    glm::mat3 view_xform_;
    glm::mat3 camwin_to_ndc_xform_;
    glm::mat3 world_to_ndc_xform_;

    void CameraInit();
    void CameraUpdate();

public:

    void Init();
    void Update(float frametime);
    void Draw() override;
    void CleanUp();

    //returns the name of the system for debug use
    virtual std::string GetName();

    //function more akin to "What to do when message is received" for internal logic
    virtual void SendMessageD(Message* m);

    using RendererIt = std::unordered_map<EntityID, Renderer>::iterator;
    void AddRendererComponent(EntityID id, Renderer* renderer);
    void RemoveRendererComponent(EntityID id);

    using AnimRendererIt = std::unordered_map<EntityID, AnimationRenderer>::iterator;
    void AddAnimationRendererComponent(EntityID id, AnimationRenderer* animation_renderer);
    void RemoveAnimationRendererComponent(EntityID id);

    void TempMoveCamera();

    std::unordered_map<EntityID, Renderer> renderer_arr_;
    std::unordered_map<EntityID, AnimationRenderer> anim_renderer_arr_;
};

#endif
