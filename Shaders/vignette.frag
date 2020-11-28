#version 450 core

layout(location=0) in vec2 vTexCoord;

layout (location=0) out vec4 fFragClr;

uniform vec2 center;
uniform vec2 clear_size;
uniform vec2 max_size;
uniform sampler2D uTex2d;
uniform float opacity;

void main () {
	
	float dist = abs(distance(gl_FragCoord.xy, center));

	float current_angle =
		acos(dot(vec2(1.0f, 0.0f), gl_FragCoord.xy - center) / dist);

	float opposite = clear_size.x * sin(current_angle);
	float adjacent = clear_size.y * cos(current_angle);

	float clear_dist = abs((clear_size.x * clear_size.y) / sqrt(opposite * opposite + adjacent * adjacent));

	opposite = max_size.x * sin(current_angle);
	adjacent = max_size.y * cos(current_angle);
	
	float max_dist = abs((max_size.x * max_size.y) / sqrt(opposite * opposite + adjacent * adjacent));
	
	if (dist < clear_dist)
	{
		fFragClr = texture(uTex2d, vTexCoord) * opacity;
	}
	
	else if (dist > max_dist)
	{
		fFragClr = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	else
	{
		float intensity = 1 - (dist - clear_dist) / (max_dist - clear_dist);
		intensity *= intensity;
		fFragClr = texture(uTex2d, vTexCoord) * opacity * intensity;
	}
}