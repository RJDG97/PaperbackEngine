#version 450 core

layout (location=0) in vec2 vVertexPosition;
layout (location=1) in vec2 vVertexTexCoord;
layout (location=2) in float texture_id;
layout (location=3) in vec3 text_color;
layout (location=4) in float opacity;

layout (location=0) out vec2 vTexCoord;
layout (location=1) out float tex_id;
layout (location=2) out vec3 tex_color;
layout (location=3) out float alpha;

uniform mat4 projection;

void main() {
        
    gl_Position = projection * vec4(vVertexPosition, 0.0, 1.0);
    vTexCoord = vVertexTexCoord;
    tex_id = texture_id;
    tex_color = text_color;
    alpha = opacity;
}
