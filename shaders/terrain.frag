#version 330 core

uniform sampler2D texture_depth;
uniform vec3 uSunPosition;
uniform mat4 uRotatedSun;
uniform int uFrames;
uniform float uZenith;
uniform vec3 uCameraPosition;
uniform float uZenithalOpticalLengthAir;
uniform float uZenithalOpticalLengthHaze;

in vec3 fragPosition;
in vec3 fragColor;
in vec4 fragPositionLightSpace;
in vec3 fragNormal;

vec3 beta_R = vec3(6.95e-6, 1.18e-5, 2.44e-5);
vec3 beta_M = vec3(2e-5, 3e-5, 4-5);
const float g = -1;
vec3 E_sun = vec3(255.0, 255.0, 255.0);

float CloudsShadowScalar()
{
	// perform perspective divide
	vec3 projCoords = fragPositionLightSpace.xyz / fragPositionLightSpace.w; //[-1;1]
	projCoords = projCoords * 0.5 + 0.5; //[0;1]
	// texture saved depth
	float textureDepth = texture(texture_depth, projCoords.xy).r;
	// current fragment depth
	float currentDepth = projCoords.z;

	//if currentDepth is less than textureDepth, the fragment is lit
	float shadow = currentDepth < textureDepth ? 1.0 : textureDepth;
	return (shadow);
}

float TerrainShadow()
{
	// perform perspective divide
	vec3 projCoords = fragPositionLightSpace.xyz / fragPositionLightSpace.w; //[-1;1]
	projCoords = projCoords * 0.5 + 0.5; //[0;1]
	// texture saved depth
	float textureDepth = texture(texture_depth, projCoords.xy).r;
	// current fragment depth
	float currentDepth = projCoords.z;

	//if currentDepth is less than textureDepth, the fragment is lit
	float shadow = currentDepth - 0.005 < textureDepth ? 1.0 : 0.0;
	return (shadow);
}

vec3 ACESFilm( vec3 x )
{
    float tA = 2.51;
    float tB = 0.03;
    float tC = 2.43;
    float tD = 0.59;
    float tE = 0.14;
    return clamp((x*(tA*x+tB))/(x*(tC*x+tD)+tE),0.0,1.0);
}

void main()
{
	vec3 sun_position = vec3(vec4(uRotatedSun * vec4(uSunPosition, 1.0)).rgb);
	vec3 color = vec3(86.0, 125.0, 70.0) / 255.0;
	vec3 light_dir = normalize(sun_position - uCameraPosition);
	vec3 view_dir = normalize(fragPosition - uCameraPosition);
	float view_dist = length(fragPosition - uCameraPosition);
	float cos_theta = dot(light_dir, view_dir);
	const float pi = 3.14159265;

	// Scattering coefficients (quantity) multiplied by their phase function (angular direction)
	vec3 B_scAir = beta_R * (3.f / (16.f * pi) * (1.f + cos_theta * cos_theta));
	vec3 B_scHaze = beta_M * (pow(1.0 - g, 2.f) / ((4.f * pi) * pow(1.f + g*g - 2.f * g * cos_theta, 1.5)));

	// Diffuse
	float diffuse = max(0.0, dot(fragNormal, light_dir));
	float shadow = CloudsShadowScalar();
	color *= shadow * diffuse;

	color = ACESFilm(color);
	color = pow(color, vec3(2.2));

	if (light_dir.y < 0.0) // earth shadow
		color *= exp(-1.0 * -light_dir.y);

	gl_FragColor = vec4(color, 1.0);
}