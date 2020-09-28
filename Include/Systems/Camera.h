#pragma once

struct Camera2D
{
    GLObject* pgo; // pointer to game object that embeds camera
    glm::vec2 right, up;
    glm::mat3 view_xform;

    //window paramenters
    GLint height{ 1000 };
    GLfloat ar;

    glm::mat3 camwin_to_ndc_xform;
    glm::mat3 world_to_ndc_xform;

    // window change parameters ...
    GLint min_height{ 500 }, max_height{ 2000 };
    // height is increasing if 1 and decreasing if -1
    GLint height_chg_dir{ 1 };
    // increments by which window height is changed per Z key press
    GLint height_chg_val{ 5 };

    // camera's speed when button U is pressed
    GLfloat linear_speed{ 2.f };

    bool first_person_cam = false;

    // keyboard button press flags
    GLboolean camtype_flag{ GL_FALSE };    // button V
    GLboolean zoom_flag{ GL_FALSE };       // button Z
    GLboolean left_turn_flag{ GL_FALSE };  // button H
    GLboolean right_turn_flag{ GL_FALSE }; // button K
    GLboolean move_flag{ GL_FALSE };       // button U


    // you can implement these functions as you wish ...
    void init(GLFWwindow* pWindow, GLObject* ptr);
    void update(GLFWwindow* pWindow);
};