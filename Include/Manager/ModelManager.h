#pragma once

#ifndef _MODELMANAGER_H_
#define _MODELMANAGER_H_

#include <GL/glew.h> // for access to OpenGL API declarations
#include <GLFW/glfw3.h>
#include "Manager/ShaderManager.h"
#include "Manager/IManager.h"

struct Model {

    GLenum primitive_type_;
    GLuint primitive_cnt_;
    GLuint vaoid_;
    GLuint vboid_;
    GLuint draw_cnt_;

    size_t vbo_tex_offset_;

    GLuint GetVBOHandle();
    size_t GetVBOTexOffset();
};

class ModelManager : public IManager {

    std::map<std::string, Model> models_;

public:

/******************************************************************************/
/*!
    \fn Init()

    \brief Initializes the Model Manager
*/
/******************************************************************************/
    void Init() override;

/******************************************************************************/
/*!
    \fn AddTristripsBatchModel(int batch_size, std::string model_name)

    \brief Adds a triangle strips model (with batch_size being the number of
           objects rendered) and adds the model to the Model Manager's map
*/
/******************************************************************************/
    Model* AddTristripsBatchModel(int batch_size, std::string model_name);

/******************************************************************************/
/*!
    \fn AddTristripsModel(int slices, int stacks, std::string model_name)

    \brief Adds a triangle strips model (with slices being the number of rows
           and stacks being the number of columns) and adds the model to the
           Model Manager's map
*/
/******************************************************************************/
    Model* AddTristripsModel(int slices, int stacks, std::string model_name);

/******************************************************************************/
/*!
    \fn AddLinesModel(std::string model_name, size_t size)

    \brief Adds a lines model, with size being number of lines drawn, and adds
           the model to the Model Manager's map
*/
/******************************************************************************/
    Model* AddLinesModel(std::string model_name, size_t size);

/******************************************************************************/
/*!
    \fn GetModel(std::string model_name)

    \brief Retrieves the model with the name specified in model_name
*/
/******************************************************************************/
    Model* GetModel(std::string model_name);

};

#endif