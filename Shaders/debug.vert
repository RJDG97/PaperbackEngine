#version 450 core

layout (location=0) in vec2 vVertexPosition;

uniform mat3 world_to_ndc_xform;

void main() {
    
    gl_Position = vec4(vVertexPosition, 0.0, 1.0);

    gl_Position = vec4(vec2(world_to_ndc_xform * vec3(vVertexPosition, 1.0f)), 0.0, 1.0);
}