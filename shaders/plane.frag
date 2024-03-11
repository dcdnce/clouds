#version 330 core

uniform vec3 uSunPosition;
uniform mat4 uRotatedSun;

in vec3 fragPosition;
in vec3 fragColor;

void main()
{
	vec3 sun_position = vec3(vec4(uRotatedSun * vec4(uSunPosition, 1.0)).rgb);
	vec3 normal = vec3(0.0, 1.0, 0.0);
	float i = dot(normal, normalize(sun_position - fragPosition));

	gl_FragColor = vec4(i, i, i, 1.0);
}