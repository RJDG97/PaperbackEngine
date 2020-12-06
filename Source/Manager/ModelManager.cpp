/**********************************************************************************
*\file         ModelManager.cpp
*\brief        Contains definition of functions and variables used for
*			   the Model Manager
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Manager/ModelManager.h"
#include "Systems/Debug.h"

void ModelManager::Init() {

    M_DEBUG->WriteDebugMessage("Model Manager Init\n");
}

Model* ModelManager::AddTristripsBatchModel(int batch_size, std::string model_name)
{
    GLuint vao_batch_;
    GLuint vbo_batch_;
    GLuint ebo_batch_;

    std::vector<glm::vec2> pos_vtx{ {-1.0f, -1.0f},
                                    { 1.0f, -1.0f},
                                    {-1.0f,  1.0f},
                                    { 1.0f,  1.0f}, };

    std::vector<GLushort> idx_vtx_sent;
    std::vector<glm::vec2> pos_vtx_sent;

    for (int i = 0; i < batch_size; ++i) {

        for (int j = 0; j < 4; ++j) {

            pos_vtx_sent.push_back(pos_vtx[j]);
        }

        idx_vtx_sent.push_back(static_cast<GLushort>(2 + 4 * i));
        idx_vtx_sent.push_back(static_cast<GLushort>(0 + 4 * i));
        idx_vtx_sent.push_back(static_cast<GLushort>(3 + 4 * i));
        idx_vtx_sent.push_back(static_cast<GLushort>(1 + 4 * i));

        if (i < batch_size - 1) {

            idx_vtx_sent.push_back(static_cast<GLushort>(1 + 4 * i));
            idx_vtx_sent.push_back(static_cast<GLushort>(2 + 4 * (i + 1)));
        }
    }

    glCreateBuffers(1, &vbo_batch_);

    //Render 500 objects at once
    glNamedBufferStorage(vbo_batch_, (sizeof(glm::vec2) * 5 + sizeof(float)) * 4 * batch_size,
        nullptr, GL_DYNAMIC_STORAGE_BIT);

    glNamedBufferSubData(vbo_batch_, 0,
        sizeof(glm::vec2) * pos_vtx_sent.size(),
        pos_vtx_sent.data());

    glCreateVertexArrays(1, &vao_batch_);

    size_t offset = 0;

    glEnableVertexArrayAttrib(vao_batch_, 0);
    glVertexArrayVertexBuffer(vao_batch_, 0, vbo_batch_, offset, sizeof(glm::vec2));
    glVertexArrayAttribFormat(vao_batch_, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_batch_, 0, 0);

    offset += sizeof(glm::vec2) * 4 * batch_size;

    glEnableVertexArrayAttrib(vao_batch_, 1);
    glVertexArrayVertexBuffer(vao_batch_, 1, vbo_batch_, offset, sizeof(glm::vec2));
    glVertexArrayAttribFormat(vao_batch_, 1, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_batch_, 1, 1);

    offset += sizeof(glm::vec2) * 4 * batch_size;

    //scaling
    glEnableVertexArrayAttrib(vao_batch_, 2);
    glVertexArrayVertexBuffer(vao_batch_, 2, vbo_batch_, offset, sizeof(glm::vec2));
    glVertexArrayAttribFormat(vao_batch_, 2, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_batch_, 2, 2);

    offset += sizeof(glm::vec2) * 4 * batch_size;

    //rotation
    glEnableVertexArrayAttrib(vao_batch_, 3);
    glVertexArrayVertexBuffer(vao_batch_, 3, vbo_batch_, offset, sizeof(glm::vec2));
    glVertexArrayAttribFormat(vao_batch_, 3, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_batch_, 3, 3);

    offset += sizeof(glm::vec2) * 4 * batch_size;

    //translation
    glEnableVertexArrayAttrib(vao_batch_, 4);
    glVertexArrayVertexBuffer(vao_batch_, 4, vbo_batch_, offset, sizeof(glm::vec2));
    glVertexArrayAttribFormat(vao_batch_, 4, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_batch_, 4, 4);

    offset += sizeof(glm::vec2) * 4 * batch_size;

    //texture id
    glEnableVertexArrayAttrib(vao_batch_, 5);
    glVertexArrayVertexBuffer(vao_batch_, 5, vbo_batch_, offset, sizeof(float));
    glVertexArrayAttribFormat(vao_batch_, 5, 1, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_batch_, 5, 5);

    glCreateBuffers(1, &ebo_batch_);
    glNamedBufferStorage(ebo_batch_,
        sizeof(GLushort) * (6 * batch_size - 2),
        idx_vtx_sent.data(),
        GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayElementBuffer(vao_batch_, ebo_batch_);
    glBindVertexArray(0);

    Model mdl;
    mdl.vaoid_ = vao_batch_;
    mdl.vboid_ = vbo_batch_;

    //mdl.vbo_tex_offset_ = sizeof(glm::vec2) * pos_vtx.size(); NOT USED
    mdl.primitive_type_ = GL_TRIANGLE_STRIP;
    mdl.draw_cnt_ = static_cast<GLint>(idx_vtx_sent.size());     // number of vertices
    //mdl.primitive_cnt_ = count;          NOT USED              // number of triangles
    models_[model_name] = mdl;

    return &models_[model_name];
}

Model* ModelManager::AddTristripsModel(int slices, int stacks, std::string model_name) {

    // Generates the vertices required to render triangle strips

    int const count{ (stacks + 1) * (slices + 1) };
    std::vector<glm::vec2> pos_vtx(count);
    std::vector<glm::vec2> tex_vtx(count);

    float const u{ 2.f / static_cast<float>(slices) };
    float const v{ 2.f / static_cast<float>(stacks) };

    for (int row{ 0 }, index{ 0 }; row <= stacks; ++row) {

        for (int col{ 0 }; col <= slices; ++col) {

            pos_vtx[index] = glm::vec2(u * static_cast<float>(col) - 1.f, v* static_cast<float>(row) - 1.f);

            tex_vtx[index++] = glm::vec2{ col, row };
        }
    }

    // Generate the triangle strip's index or element list

    std::vector<GLushort> idx_vtx(((slices + 1) * 2 + 2)* stacks - 2);

    for (int row{ 0 }, index{ 0 }; row <= stacks - 1; ++row) {

        for (int col{ 0 }; col <= slices; ++col) {

            if (index != 0 && col == 0) {

                idx_vtx[index++] = static_cast<GLushort>((row + 1) * (slices + 1) + col);
            }

            idx_vtx[index++] = static_cast<GLushort>((row + 1) * (slices + 1) + col);
            idx_vtx[index++] = static_cast<GLushort>(row * (slices + 1) + col);
        }

        if (row != stacks - 1) {

            idx_vtx[index] = idx_vtx[index - 1];
            ++index;
        }
    }

    // Generate a VAO handle to encapsulate the VBO(s) and state of this triangle mesh 

    GLuint vbo_hdl;
    glCreateBuffers(1, &vbo_hdl);


    glNamedBufferStorage(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size() +
                            sizeof(glm::vec2) * tex_vtx.size(),
                            nullptr, GL_DYNAMIC_STORAGE_BIT);

    glNamedBufferSubData(vbo_hdl, 0, sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data());

    glNamedBufferSubData(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(),
                         sizeof(glm::vec2) * tex_vtx.size(), tex_vtx.data());

    size_t offset = 0;

    GLuint vao_hdl;
    glCreateVertexArrays(1, &vao_hdl);

    glEnableVertexArrayAttrib(vao_hdl, 0);
    glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, offset, sizeof(glm::vec2));
    glVertexArrayAttribFormat(vao_hdl, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_hdl, 0, 0);

    offset += sizeof(glm::vec2) * pos_vtx.size();

    glEnableVertexArrayAttrib(vao_hdl, 1);
    glVertexArrayVertexBuffer(vao_hdl, 1, vbo_hdl, offset, sizeof(glm::vec2));
    glVertexArrayAttribFormat(vao_hdl, 1, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_hdl, 1, 1);

    GLuint ebo_hdl;
    glCreateBuffers(1, &ebo_hdl);
    glNamedBufferStorage(ebo_hdl,
                         sizeof(GLushort) * idx_vtx.size(),
                         reinterpret_cast<GLvoid*>(idx_vtx.data()),
                         GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayElementBuffer(vao_hdl, ebo_hdl);
    glBindVertexArray(0);

    // Return an appropriately initialized instance of GLApp::GLModel

    Model mdl;
    mdl.vaoid_ = vao_hdl;
    mdl.vboid_ = vbo_hdl;

    mdl.vbo_tex_offset_ = sizeof(glm::vec2) * pos_vtx.size();
    mdl.primitive_type_ = GL_TRIANGLE_STRIP;
    mdl.draw_cnt_ = static_cast<GLint>(idx_vtx.size());     // number of vertices
    mdl.primitive_cnt_ = count;                             // number of triangles
    models_[model_name] = mdl;

    return &models_[model_name];
}

Model* ModelManager::AddLinesModel(std::string model_name, size_t size) {

    // Sets the position of the start and end of each line in a line model

    std::vector<glm::vec2> pos_vtx;
    std::vector<GLushort> idx_vtx;

    for (size_t i = 0; i < size * 2; ++i)
    {
        idx_vtx.push_back(static_cast<GLushort>(i));
        pos_vtx.push_back({ static_cast<GLushort>(i), static_cast<GLushort>(i) });    //pos_vtx stuff doesn't matter since it will be data will be changed later on
        pos_vtx.push_back({ static_cast<GLushort>(i), static_cast<GLushort>(i) });
    }

    // Generate a VAO handle to encapsulate the VBO(s)

    GLuint vbo_hdl;
    glCreateBuffers(1, &vbo_hdl);
    glNamedBufferStorage(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(),
        pos_vtx.data(), GL_DYNAMIC_STORAGE_BIT);

    GLuint vao_hdl;
    glCreateVertexArrays(1, &vao_hdl);

    glEnableVertexArrayAttrib(vao_hdl, 0);
    glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(glm::vec2));
    glVertexArrayAttribFormat(vao_hdl, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_hdl, 0, 0);

    GLuint ebo_hdl;
    glCreateBuffers(1, &ebo_hdl);
    glNamedBufferStorage(ebo_hdl,
        sizeof(GLushort) * idx_vtx.size(),
        reinterpret_cast<GLvoid*>(idx_vtx.data()),
        GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayElementBuffer(vao_hdl, ebo_hdl);
    glBindVertexArray(0);

    Model mdl;
    mdl.vaoid_ = vao_hdl;
    mdl.vboid_ = vbo_hdl;
    mdl.primitive_type_ = GL_LINES;
    mdl.draw_cnt_ = static_cast<GLuint>(idx_vtx.size());    // number of vertices
    mdl.primitive_cnt_ = static_cast<GLuint>(idx_vtx.size()) / 2;                // number of GL_LINES
    models_[model_name] = mdl;

    return &models_[model_name];
}

Model* ModelManager::GetModel(std::string model_name) {

    return &models_[model_name];
}

GLuint Model::GetVBOHandle()
{
    return vboid_;
}

size_t Model::GetVBOTexOffset()
{
    return vbo_tex_offset_;
}
