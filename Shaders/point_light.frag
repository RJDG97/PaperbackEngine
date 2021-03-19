#version 450 core

layout (location=0) in vec2 center;
layout (location=1) in vec3 color;
layout (location=2) in float intensity;
layout (location=3) in float radius;

layout (location=0) out vec4 fFragClr;

void main () {

  float dist = distance(gl_FragCoord.xy, center);
  float attenuation = max(0, 1 - dist/radius);
  attenuation *= attenuation;
  fFragClr = intensity * attenuation * vec4(color, 1.0);
}