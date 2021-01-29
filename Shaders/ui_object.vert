#version 450 core

layout (location=0) in vec2 vVertexPosition;
layout (location=1) in vec2 vVertexTexCoord;
layout (location=2) in vec2 scaling;
layout (location=3) in vec2 rotation;
layout (location=4) in vec2 position;
layout (location=5) in float texture_id;
layout (location=6) in float opacity;
layout (location=7) in vec3 color;

layout (location=0) out vec2 vTexCoord;
layout (location=1) out float tex_id;
layout (location=2) out float alpha;
layout (location=3) out vec3 tint;

uniform mat4 projection;

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

    mat3 uModel_to_NDC = trans * rot * scal;

    gl_Position = projection * vec4( vec2( uModel_to_NDC * vec3( vVertexPosition, 1.0f)), 0.0, 1.0);
    vTexCoord = vVertexTexCoord;
    tex_id = texture_id;
    alpha = opacity;
    tint = color;
}
