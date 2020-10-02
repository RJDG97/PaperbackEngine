/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GLSLSHADER_H
#define GLSLSHADER_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Manager/IManager.h"
#include <GL/glew.h> // for access to OpenGL API declarations 
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>

/*  _________________________________________________________________________ */

class Shader
    /*! Shader class.
    */
{
public:

    Shader() : pgm_handle(0), is_linked(GL_FALSE) {}

    GLboolean CompileLinkValidate(std::vector<std::pair<GLenum, std::string>>);
    GLboolean CompileShaderFromFile(GLenum shader_type, std::string const& file_name);
    GLboolean CompileShaderFromString(GLenum shader_type, std::string const& shader_src);
    GLboolean Link();

    void Use();
    void UnUse();

    GLboolean Validate();

    GLuint GetHandle() const;

    GLboolean IsLinked() const;

    std::string GetLog() const;

    void BindAttribLocation(GLuint index, GLchar const* name);
    void BindFragDataLocation(GLuint color_number, GLchar const* name);

    void DeleteShaderProgram();

    void SetUniform(GLchar const* name, GLboolean val);
    void SetUniform(GLchar const* name, GLint val);
    void SetUniform(GLchar const* name, GLfloat val);
    void SetUniform(GLchar const* name, GLfloat x, GLfloat y);
    void SetUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z);
    void SetUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    void SetUniform(GLchar const* name, glm::vec2& val);
    void SetUniform(GLchar const* name, glm::vec3& val);
    void SetUniform(GLchar const* name, glm::vec4& val);
    void SetUniform(GLchar const* name, glm::mat3& val);
    void SetUniform(GLchar const* name, glm::mat4& val);

    // display the list of active vertex attributes used by vertex shader
    void PrintActiveAttribs() const;
    // display the list of active uniform variables
    void PrintActiveUniforms() const;

private:
    enum ShaderType {
        VERTEX_SHADER = GL_VERTEX_SHADER,
        FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
        GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
        TESS_CONTROL_SHADER = GL_TESS_CONTROL_SHADER,
        TESS_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER,
        // ignore compute shader for now because it is not connected to
        // the graphics pipe
        // COMPUTE_SHADER = GL_COMPUTE_SHADER
    };

    GLuint pgm_handle = 0;  // handle to linked shader program object
    GLboolean is_linked = GL_FALSE; // has the program successfully linked?
    std::string log_string; // log for OpenGL compiler and linker messages

    // use OpenGL API to return the location of an uniform variable with
    // name "name" using program handle encapsulated by object of this class type
    GLint GetUniformLocation(GLchar const* name);

    // return true if file (given in relative path) exists, false otherwise
    GLboolean FileExists(std::string const& file_name);
};

/*
enum ShaderType
{
    TextureShader,
    LightShader
};*/

class ShaderManager : public IManager
{
    std::map<std::string, Shader> shaders_;

public:

    void Init() override;

    void AddShdrpgm(std::string vtx_shdr, std::string frg_shdr, std::string shader_type);

    Shader GetShdrpgm(std::string shader_type);
};

#endif
