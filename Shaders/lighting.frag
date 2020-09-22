#version 450 core

layout(location=0) in vec3 vClrCoord;

layout (location=0) out vec4 fFragClr;

uniform vec3 light_color;
uniform vec2 light_center;
uniform float intensity;
uniform float radius;

void main () {

  float dist = distance(gl_FragCoord.xy, light_center);
  float attenuation = max(0, 1 - dist/radius);
  attenuation *= attenuation;
  fFragClr = intensity * attenuation * vec4(light_color, 1.0);
}