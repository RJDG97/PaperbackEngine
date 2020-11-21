#version 450 core

layout(location=0) in vec2 vTexCoord;

layout (location=0) out vec4 fFragClr;

uniform vec2 center;
uniform float clear_radius;
uniform float max_radius;
uniform sampler2D uTex2d;
uniform float opacity;

void main () {
	
	float dist = distance(gl_FragCoord.xy, center);
	
	if (dist < clear_radius)
	{
		fFragClr = texture(uTex2d, vTexCoord) * opacity;
	}
	
	else
	{
		float intensity = 1 - (dist - clear_radius) / (max_radius - clear_radius);
		intensity *= intensity;
		vec4 vignette_color = vec4(intensity, intensity, intensity, 1.0f);
		fFragClr = texture(uTex2d, vTexCoord) * opacity / vignette_color;
	}
}
