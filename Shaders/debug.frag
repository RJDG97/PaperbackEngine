#version 450 core

layout (location=0) out vec4 fFragClr;

uniform int collided;

void main () {

    if (collided == 1)
    {
        fFragClr = vec4(1.0, 0.0, 0.0, 1.0);
    }

    else
    {
        fFragClr = vec4(0.0, 1.0, 0.0, 1.0);
    }
}
