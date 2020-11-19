#version 450 core

layout (location=0) in vec2 vVertexPosition;

void main() {
        
    gl_Position = vec4(vVertexPosition, 0.0, 1.0);
}
