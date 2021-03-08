#version 450 core

layout(location=0) in vec2 vTexCoord;
layout(location=1) in float tex_id;
layout(location=2) in float alpha;
layout (location=3) in vec3 tint;

layout (location=0) out vec4 fFragClr;

uniform sampler2D uTex2d[20];

void main () {
    
    vec4 color =  texture(uTex2d[int(tex_id)], vTexCoord);
    color.a *= alpha;

    if (color.a <= 0.4f)
    {
        discard;
    }

    fFragClr = color * vec4(tint, 1.0);
}