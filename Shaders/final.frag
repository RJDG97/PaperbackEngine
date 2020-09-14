#version 450 core

layout(location=0) in vec3 vClrCoord;
layout(location=1) in vec2 vTexCoord;

layout (location=0) out vec4 fFragClr;

uniform sampler2D uTex2d;

void main () {

  fFragClr = texture(uTex2d, vTexCoord);// * vec4(0.25,0.2,0.35,1.0);
}
