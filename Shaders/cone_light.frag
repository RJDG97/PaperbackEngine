#version 450 core

layout (location=0) in vec2 center;
layout (location=1) in vec3 color;
layout (location=2) in float intensity;
layout (location=3) in float radius;
layout (location=4) in vec2 direction;
layout (location=5) in float angle;

layout (location=0) out vec4 fFragClr;

void main () {
  
  vec2 norm_direction = direction / length(direction);
  
  float current_angle =
	acos(dot(norm_direction, gl_FragCoord.xy - center) / distance(gl_FragCoord.xy, center));
	
  if (current_angle <= angle)
  {
	float dist = distance(gl_FragCoord.xy, center);
	float attenuation = max(0, 1 - dist/radius);

	attenuation *= attenuation * attenuation;
	float alpha = 1.0 - (current_angle/angle) * (current_angle/angle);

	if (dist < 5)
	{
		alpha *= dist/5 * dist/5;
	}

	fFragClr = alpha * intensity * attenuation * vec4(color, 1.0f);
  }

  else
  {
	fFragClr = vec4(0.0f, 0.0f, 0.0f, 0.0f);
  }
}