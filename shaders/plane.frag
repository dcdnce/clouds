#version 330 core

uniform sampler2D texture_depth;
uniform vec3 uSunPosition;
uniform mat4 uRotatedSun;

in vec3 fragPosition;
in vec3 fragColor;
in vec4 fragPositionLightSpace;

float InShadow()
{
	// perform perspective divide
	vec3 projCoords = fragPositionLightSpace.xyz / fragPositionLightSpace.w; //[-1;1]
	projCoords = projCoords * 0.5 + 0.5; //[0;1]
	// texture saved depth
	float textureDepth = texture(texture_depth, projCoords.xy).r;
	// current fragment depth
	float currentDepth = projCoords.z;

	float shadow = currentDepth < textureDepth ? 0.0 : 1.0;
	return (shadow);
}

void main()
{
	vec3 sun_position = vec3(vec4(uRotatedSun * vec4(uSunPosition, 1.0)).rgb);
	vec3 normal = vec3(0.0, 1.0, 0.0);
	float i = dot(normal, normalize(sun_position - vec3(0.0)));

	float shadow = InShadow();
	i *= (1.0 - shadow) + 0.15;
	gl_FragColor = vec4(i, i, i, 1.0);
	// gl_FragColor = vec4(fragColor, 1.0);
}