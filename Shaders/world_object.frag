#version 450 core

layout(location=0) in vec2 vTexCoord;
layout(location=1) flat in int tex_id;

layout (location=0) out vec4 fFragClr;

uniform sampler2D uTex2d[10];

void main () {

    fFragClr = texture(uTex2d[tex_id], vTexCoord);
}