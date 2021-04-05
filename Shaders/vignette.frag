#version 450 core

layout (location=0) out vec4 fFragClr;

uniform vec3 color;
uniform vec2 center;
uniform vec2 clear_size;
uniform vec2 max_size;
uniform float opacity;

void main () {
	
	float dist = abs(distance(gl_FragCoord.xy, center));

	float current_angle =
		acos(dot(vec2(1.0f, 0.0f), gl_FragCoord.xy - center) / dist);

	float clear_dist;
	float max_dist;

	if (length(clear_size) <= 0.0f)
	{
		clear_dist = 0.0f;
	}

	else
	{
		float opposite = clear_size.x * sin(current_angle);
		float adjacent = clear_size.y * cos(current_angle);

		clear_dist = abs((clear_size.x * clear_size.y) / sqrt(opposite * opposite + adjacent * adjacent));
	}

	if (length(max_size) <= 0.0f)
	{
		max_dist = 0.0f;
	}

	else
	{
		float opposite = max_size.x * sin(current_angle);
		float adjacent = max_size.y * cos(current_angle);
	
		max_dist = abs((max_size.x * max_size.y) / sqrt(opposite * opposite + adjacent * adjacent));
	}

	//Determines the color of the fragment
	
	if (dist < clear_dist)
	{
		fFragClr = vec4(color, 0.0f) * opacity;
	}
	
	else if (dist >= max_dist || max_dist <= 0.0f)
	{
		fFragClr = vec4(color, 1.0f) * opacity;
	}

	else
	{
		float intensity;

		if (clear_size.x <= 0.0f)
		{

			intensity = dist / max_dist;
		}

		else
		{
			intensity = (dist - clear_dist) / (max_dist - clear_dist);
		}

		intensity *= intensity;
		fFragClr = vec4(color, intensity) * opacity;
	}
}