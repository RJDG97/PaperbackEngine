#version 450 core

layout (location=0) in vec2 vVertexPosition;

uniform mat3 uModel_to_NDC;

void main() {
    gl_Position = vec4(vec2(uModel_to_NDC * vec3(vVertexPosition, 1.0f)), 0.0, 1.0);
}
