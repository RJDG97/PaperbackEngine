#pragma once

#ifndef _MODELMANAGER_H_
#define _MODELMANAGER_H_

#include <GL/glew.h> // for access to OpenGL API declarations
#include <GLFW/glfw3.h>
#include "Manager/ShaderManager.h"
#include "Manager/IManager.h"

enum ModelType
{
    BoxModel
};

struct Model
{
    GLenum     primitive_type_;
    GLuint     primitive_cnt_;
    GLuint     vaoid_;
    GLuint     draw_cnt_;
};

class ModelManager : public IManager
{
    std::map<GLint, Model> models_;

public:

    void Init() override;

    void AddTristripsModel(int slices, int stacks, GLint model_type);

    Model GetModel(GLint model_id);

};

#endif