#pragma once

#ifndef _MODELMANAGER_H_
#define _MODELMANAGER_H_

#include <GL/glew.h> // for access to OpenGL API declarations
#include <GLFW/glfw3.h>
#include "ShaderManager.h"


enum ModelType
{
    BoxModel
};

struct Model
{
    GLenum     primitive_type;
    GLuint     primitive_cnt;
    GLuint     vaoid;
    GLuint     draw_cnt;
};

class ModelManager
{
    std::map<GLint, Model> models;

public:

    void AddTristripsModel(int slices, int stacks, ModelType model_type);

    Model GetModel(GLint model_id);

    ModelManager();

};

extern ModelManager* MODELMANAGER;

#endif