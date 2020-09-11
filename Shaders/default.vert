#version 450 core

layout (location=0) in vec2 vVertexPosition;
layout (location=1) in vec3 vVertexClrCoord;
layout (location=2) in vec2 vVertexTexCoord;

layout (location=0) out vec3 vClrCoord;
layout (location=1) out vec2 vTexCoord;

uniform int task;

void main() {
        
    gl_Position = vec4(vVertexPosition, 0.0, 1.0);
    vClrCoord = vVertexClrCoord;
    vTexCoord = vVertexTexCoord;
}
