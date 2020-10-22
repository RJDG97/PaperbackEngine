#version 450 core

layout(location=0) in vec2 vTexCoord;

layout (location=0) out vec4 fFragClr;

uniform sampler2D uTex2d;
uniform vec3 textColor;

void main () {
    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(uTex2d, vTexCoord).r);
    fFragClr = vec4(textColor, 1.0) * sampled);
}