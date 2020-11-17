#version 450 core

layout (location=0) out vec4 fFragClr;

uniform vec4 color;

void main () {

	fFragClr = color;
}
