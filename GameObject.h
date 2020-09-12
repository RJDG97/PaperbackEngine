#pragma once

#include <glm/gtc/type_ptr.hpp>
#include "Vector2D.h"
#include "Matrix3x3.h"
#include "GraphicsSystem.h"
#include <map>
#include <string>

//EVERYTHING HERE IS JUST USED TO TEST THE CAMERA TRANSFORMS
//WE WONT USE THIS

struct GLObject
{
    // angle_speed: rate of change of rotation angle per second
    // angle_disp: current absolute orientation angle
    // all angles refer to rotation about Z-axis
    Vector2D orientation;

    // scaling parameters
    Vector2D scaling;

    //translation vector coordinates
    Vector2D position;

    //compute object's model transform matrix using
    //scale rotate translate
    //computed by the CPU not the vertex shader
    glm::mat3 mdl_to_ndc_xform; // model-to-NDC transform

    std::map<std::string, GraphicsSystem::GLModel>::iterator mdl_ref;
    std::map<std::string, GLSLShader>::iterator     shd_ref;

    glm::mat3 mdl_xform; // model (model-to-world) transform 

    void init(Vector2D object_orientation, Vector2D object_scaling, Vector2D object_position);

    //render the object's model
    void draw() const;

    //update object's model transformation matrix
    void update(double delta_time);
};