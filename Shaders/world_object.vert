#version 450 core

layout (location=0) in vec2 vVertexPosition;
layout (location=1) in vec2 vVertexTexCoord;

layout (location=0) out vec2 vTexCoord;

uniform mat3 uModel_to_NDC;
uniform vec2 offset;

void main() {
        
    gl_Position = vec4(vec2(uModel_to_NDC * vec3(vVertexPosition, 1.f)), 0.0, 1.0);
    vTexCoord = vVertexTexCoord;
}
