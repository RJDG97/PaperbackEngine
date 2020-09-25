/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GRAPHICSSYSTEM_H
#define GRAPHICSSYSTEM_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <GL/glew.h> // for access to OpenGL API declarations
#include <GLFW/glfw3.h>
#include "glslshader.h"
#include "TextureManager.h"
#include "AnimationManager.h"
#include "ISystem.h"

class GraphicsSystem : public ISystem
{
    // encapsulates state required to render a geometrical model
    struct Model
    {
        GLenum     primitive_type;
        GLuint     primitive_cnt;
        GLuint     vaoid;

        GLuint     draw_cnt;
        GLSLShader shdr_pgm;

        // member functions defined in glapp.cpp
        void SetupShdrpgm(std::string vtx_shdr, std::string frg_shdr);
        void Draw(GLuint texID);
        void DrawLight(glm::vec3 light_color, glm::vec2 light_center,
                       float intensity, float radius);
    };

    Model TristripsModel(int slices, int stacks, std::string vtx_shdr, std::string frg_shdr);

    // handles all the textures
    static TextureManager texture_manager;

    // handles all the animations
    static AnimationManager animation_manager;

    GLint window_width;
    GLint window_height;

public:

    static TextureManager& GetTextureManager();
    static AnimationManager& GetAnimationManager();

    void Init();
    void Update(float frametime);
    void Draw() override;
    void CleanUp();

    //returns the name of the system for debug use
    virtual std::string GetName();

    //function more akin to "What to do when message is received" for internal logic
    virtual void SendMessageD(Message* m);

    std::vector<Model> models;
};

#endif
