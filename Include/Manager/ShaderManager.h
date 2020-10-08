#ifndef GLSLSHADER_H
#define GLSLSHADER_H

#include "Manager/IManager.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>

class Shader {

public:

    Shader() : pgm_handle(0), is_linked(GL_FALSE) {}

/******************************************************************************/
/*!
    \fn CompileLinkValidate(std::vector<std::pair<GLenum, std::string>>)

    \brief Compiles an individual shader and then links multiple shader
           objects to create an executable shader program.
            For each shader source, the function requires the full path
            to the file containing the shader and the type of shader program.
*/
/******************************************************************************/
    void CompileLinkValidate(std::vector<std::pair<GLenum, std::string>> vec);

/******************************************************************************/
/*!
    \fn CompileShaderFromFile(GLenum shader_type, std::string const& file_name)

    \brief Loads up a shader source code using file_name and compiles it.
           Then, create a shader program object and attach the compiled code
           onto the object.
*/
/******************************************************************************/

    GLboolean CompileShaderFromFile(GLenum shader_type, std::string const& file_name);

/******************************************************************************/
/*!
    \fn CompileShaderFromFile(GLenum shader_type, std::string const& file_name)

    \brief Compiles a shader source code and then attaches it to the
           shader program object
*/
/******************************************************************************/
    GLboolean CompileShaderFromString(GLenum shader_type, std::string const& shader_src);

/******************************************************************************/
/*!
    \fn Link()

    \brief Link shader objects attached to handle pgm_handle
*/
/******************************************************************************/
    void Link();

/******************************************************************************/
/*!
    \fn Use()

    \brief Install the shader program object whose handle is encapsulated
           by member pgm_handle
*/
/******************************************************************************/
    void Use();

/******************************************************************************/
/*!
    \fn UnUse()

    \brief De-install previously installed shader program object using Use()
*/
/******************************************************************************/
    void UnUse();

/******************************************************************************/
/*!
    \fn Validate()

    \brief Check whether the executable shader program object can execute
           given the current OpenGL state
*/
/******************************************************************************/
    void Validate();

/******************************************************************************/
/*!
    \fn GetHandle()

    \brief Return the handle to the shader program object
*/
/******************************************************************************/
    GLuint GetHandle() const;

/******************************************************************************/
/*!
    \fn IsLinked()

    \brief Returns whether the handle to the shader program object is linked
*/
/******************************************************************************/
    GLboolean IsLinked() const;

/******************************************************************************/
/*!
    \fn DeleteShaderProgram()

    \brief Deletes the shader program object
*/
/******************************************************************************/
    void DeleteShaderProgram();

/******************************************************************************/
/*!
    \fn SetUniform(GLchar const* name, GLboolean val)

    \brief Sets the values of a boolean uniform inside the shader program object
*/
/******************************************************************************/
    void SetUniform(GLchar const* name, GLboolean val);

/******************************************************************************/
/*!
    \fn SetUniform(GLchar const* name, GLint val)

    \brief Sets the values of an int uniform inside the shader program object
*/
/******************************************************************************/
    void SetUniform(GLchar const* name, GLint val);

/******************************************************************************/
/*!
    \fn SetUniform(GLchar const* name, GLfloat val)

    \brief Sets the values of a float uniform inside the shader program object
*/
/******************************************************************************/
    void SetUniform(GLchar const* name, GLfloat val);

/******************************************************************************/
/*!
    \fn SetUniform(GLchar const* name, GLfloat val)

    \brief Sets the values of a vec2 uniform inside the shader program object
*/
/******************************************************************************/
    void SetUniform(GLchar const* name, GLfloat x, GLfloat y);

/******************************************************************************/
/*!
    \fn SetUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z)

    \brief Sets the values of a vec3 uniform inside the shader program object
*/
/******************************************************************************/
    void SetUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z);

/******************************************************************************/
/*!
    \fn SetUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)

    \brief Sets the values of a vec4 uniform inside the shader program object
*/
/******************************************************************************/
    void SetUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

/******************************************************************************/
/*!
    \fn SetUniform(GLchar const* name, glm::vec2 val)

    \brief Sets the values of a vec2 uniform inside the shader program object
*/
/******************************************************************************/
    void SetUniform(GLchar const* name, glm::vec2 val);

/******************************************************************************/
/*!
    \fn SetUniform(GLchar const* name, glm::vec3 val)

    \brief Sets the values of a vec3 uniform inside the shader program object
*/
/******************************************************************************/
    void SetUniform(GLchar const* name, glm::vec3 val);

/******************************************************************************/
/*!
    \fn SetUniform(GLchar const* name, glm::vec4 val)

    \brief Sets the values of a vec4 uniform inside the shader program object
*/
/******************************************************************************/
    void SetUniform(GLchar const* name, glm::vec4 val);

/******************************************************************************/
/*!
    \fn SetUniform(GLchar const* name, glm::mat3& val)

    \brief Sets the values of a mat3 uniform inside the shader program object
*/
/******************************************************************************/
    void SetUniform(GLchar const* name, glm::mat3& val);

/******************************************************************************/
/*!
    \fn SetUniform(GLchar const* name, glm::mat4& val)

    \brief Sets the values of a mat3 uniform inside the shader program object
*/
/******************************************************************************/
    void SetUniform(GLchar const* name, glm::mat4& val);

/******************************************************************************/
/*!
    \fn PrintActiveAttribs()

    \brief display the list of active vertex attributes used by vertex shader
*/
/******************************************************************************/
    void PrintActiveAttribs() const;

/******************************************************************************/
/*!
    \fn PrintActiveUniforms()

    \brief display the list of active uniform variables used by vertex shader
*/
/******************************************************************************/
    void PrintActiveUniforms() const;

private:
    enum ShaderType {
        VERTEX_SHADER = GL_VERTEX_SHADER,
        FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
        GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
        TESS_CONTROL_SHADER = GL_TESS_CONTROL_SHADER,
        TESS_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER
    };

    GLuint pgm_handle = 0;  // handle to linked shader program object
    GLboolean is_linked = GL_FALSE; // has the program successfully linked?
};

class ShaderManager : public IManager {

    std::map<std::string, Shader> shaders_;

public:

/******************************************************************************/
/*!
    \fn Init()

    \brief Initializes the shader manager
*/
/******************************************************************************/
    void Init() override;

/******************************************************************************/
/*!
    \fn AddShdrpgm(std::string vtx_shdr, std::string frg_shdr, std::string shader_type)

    \brief Adds a shader program to the shader manager's map
*/
/******************************************************************************/
    void AddShdrpgm(std::string vtx_shdr, std::string frg_shdr, std::string shader_type);

/******************************************************************************/
/*!
    \fn GetShdrpgm(std::string shader_type)

    \brief Gets a shader program from the shader manager's map
*/
/******************************************************************************/
    Shader* GetShdrpgm(std::string shader_type);
};

#endif
