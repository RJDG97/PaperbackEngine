#version 450 core

layout(location=0) in vec2 vTexCoord;
layout(location=1) in float tex_id;
layout(location=2) in vec3 tex_color;
layout(location=3) in float alpha;

layout (location=0) out vec4 fFragClr;

uniform sampler2D uTex2d[20];

void main () {
    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(uTex2d[int(tex_id)], vTexCoord).r);
    fFragClr = vec4(tex_color, alpha) * sampled;
}