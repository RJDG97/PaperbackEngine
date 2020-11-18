#version 450 core

layout (location=0) out vec4 fFragClr;

uniform vec3 light_color;
uniform vec2 light_center;
uniform float intensity;
uniform float radius;
uniform vec2 direction;
uniform float angle;

void main () {
  
  vec2 norm_direction = direction / length(direction);
  
  float current_angle =
	acos(dot(norm_direction, gl_FragCoord.xy - light_center) / distance(gl_FragCoord.xy, light_center));
	
  if (current_angle <= angle)
  {
	float dist = distance(gl_FragCoord.xy, light_center);
	float attenuation = max(0, 1 - dist/radius);

	attenuation *= attenuation;
	float alpha = 1.0 - (current_angle/angle) * (current_angle/angle);

	if (dist < 20)
	{
		alpha *= dist/20 * dist/20;
	}

	fFragClr = alpha * intensity * attenuation * vec4(light_color, 1.0f);
  }
}