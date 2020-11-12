#version 450 core

layout(location=0) in vec2 vTexCoord;

layout (location=0) out vec4 fFragClr;

uniform sampler2D uTex2d;

void main () {
    
    vec4 color = texture(uTex2d, vTexCoord);

    if (color.a == 0.0) {
        discard; 
    }

    fFragClr = color;
}