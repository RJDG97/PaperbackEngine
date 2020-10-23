#version 450 core

layout (location=0) in vec2 vVertexPosition;
layout (location=2) in vec2 vVertexTexCoord;

layout (location=0) out vec2 vTexCoord;

uniform mat4 projection;

void main() {
        
    gl_Position = projection * vec4(vVertexPosition, 0.0, 1.0);
    vTexCoord = vVertexTexCoord;
}
