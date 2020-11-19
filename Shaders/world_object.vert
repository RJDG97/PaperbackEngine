#version 450 core

layout (location=0) in vec2 vVertexPosition;
layout (location=1) in vec2 vVertexTexCoord;
layout (location=2) in vec2 scaling;
layout (location=3) in vec2 rotation;
layout (location=4) in vec2 position;
layout (location=5) in float texture_id;

layout (location=0) out vec2 vTexCoord;
layout (location=1) flat out int tex_id;

uniform mat3 world_to_ndc_xform;

void main() {
    
    mat3 scal = mat3( scaling.x, 0.0f, 0.0f,
                      0.0f, scaling.y, 0.0f,
                      0.0f, 0.0f, 1.0f );

    mat3 rot = mat3( rotation.x, rotation.y, 0.0f,
                    -rotation.y, rotation.x, 0.0f,
                     0.0f, 0.0f, 1.0f );

    mat3 trans = mat3( 1.0f, 0.0f, 0.0f,
                       0.0f, 1.0f, 0.0f,
                       position.x, position.y, 1.0f );

    mat3 uModel_to_NDC = world_to_ndc_xform * trans * rot * scal;

    gl_Position = vec4(vec2(uModel_to_NDC * vec3(vVertexPosition, 1.f)), 0.0, 1.0);
    vTexCoord = vVertexTexCoord;
    tex_id = int(texture_id);
}