#version 450 core

layout (location=0) in vec2 vVertexPosition;
layout (location=1) in vec2 light_center;
layout (location=2) in vec3 light_color;
layout (location=3) in float light_intensity;
layout (location=4) in float light_radius;
layout (location=5) in vec2 light_direction;
layout (location=6) in float light_angle;

layout (location=0) out vec2 center;
layout (location=1) out vec3 color;
layout (location=2) out float intensity;
layout (location=3) out float radius;
layout (location=4) out vec2 direction;
layout (location=5) out float angle;

void main() {
        
    gl_Position = vec4(vVertexPosition, 0.0, 1.0);
    center = light_center;
    color = light_color;
    intensity = light_intensity;
    radius = light_radius;
    direction = light_direction;
    angle = light_angle;
}
