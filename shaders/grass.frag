#version 330 core

smooth in vec2 geomTexCoord;
smooth in vec3 geomWorldPos;
smooth in vec4 geomEyeSpacePos;
in vec4 geomPositionLightSpace;

uniform vec3 uSunPosition;
uniform mat4 uRotatedSun;

uniform sampler2D texture_grass;
uniform sampler2D texture_depth;

vec3 E_sun = vec3(250.0, 235.0, 200.0);

float CloudsShadowScalar()
{
	// perform perspective divide
	vec3 projCoords = geomPositionLightSpace.xyz / geomPositionLightSpace.w; //[-1;1]
	projCoords = projCoords * 0.5 + 0.5; //[0;1]
	// texture saved depth
	float textureDepth = texture(texture_depth, projCoords.xy).r;
	// current fragment depth
	float currentDepth = projCoords.z;

	float shadow = currentDepth < textureDepth ? 1.0 : textureDepth;
	// float shadow = currentDepth < textureDepth ? 1.0 : 0.0;
	return (shadow);
}

void main()
{
	// Grass texturing
	vec4 color = texture(texture_grass, vec2(geomTexCoord.x, 1.0 - geomTexCoord.y));
	color.a = min(1.0, color.a * 1.25);
	if (color.a < 0.25)
		// color.a = 0.0;
		discard ;

	color.rgb *= vec3(86.0, 125.0, 70.0) / 255.0;
	// Light and clouds shadow
	vec3 sun_position = vec3(vec4(uRotatedSun * vec4(uSunPosition, 1.0)).rgb);
	vec3 normal = vec3(0.0, 1.0, 0.0);
	vec3 ambient = 0.9 * (E_sun/255.0);
	vec3 diffuse = (E_sun/255.0) * dot(normal, normalize(sun_position - vec3(0.0, 6000.0, 0.0)));
	float shadow = CloudsShadowScalar();
	vec3 light = ambient + shadow * diffuse;    
	// vec3 light = shadow * diffuse;    

	gl_FragColor = color * vec4(light, 1.0);
}