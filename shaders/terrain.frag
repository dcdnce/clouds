#version 330 core

uniform sampler2D texture_depth;
uniform vec3 uSunPosition;
uniform mat4 uRotatedSun;
uniform int uFrames;
uniform float uZenith;
uniform vec3 uCameraPosition;
uniform float uZenithalOpticalLengthAir;
uniform float uZenithalOpticalLengthHaze;
uniform float uG;

in vec3 fragPosition;
in vec3 fragColor;
in vec4 fragPositionLightSpace;
in vec3 fragNormal;

vec3 beta_R = vec3(6.95e-6, 1.18e-5, 2.44e-5);
vec3 beta_M = vec3(2e-5, 3e-5, 4-5);
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
	vec3 light_dir = normalize(fragPosition - sun_position);
	vec3 view_dir = normalize(uCameraPosition - fragPosition);
	float view_dist = length(uCameraPosition - fragPosition);
	const float pi = 3.14159265;

	// SUNLIGHT
	float cos_theta = clamp(dot(normalize(sun_position - uCameraPosition), vec3(0.f, 1.f, 0.f)), 0.f, 1.f);
	float theta_degree = acos(cos_theta) * 180.f / pi;
	float air_mass = 1.0 / (cos_theta + 0.15 * pow(93.885 - theta_degree, -1.253));
	air_mass *= exp(6.5f * (1.0 - cos_theta)); // heuristic exponential - BAD
	float sAir = uZenithalOpticalLengthAir * air_mass;
	float sHaze = uZenithalOpticalLengthHaze * air_mass;
	vec3 F_ex = exp(-(beta_R*sAir));
	E_sun *= F_ex;

	// Diffuse
	float diffuse = clamp(dot(fragNormal, normalize(sun_position - fragPosition)), 0.f, 1.f);
	float shadow = CloudsShadowScalar();
	color *= shadow * diffuse * cos_theta;

	// GENERAL EQUATION - AERIAL PERSPECTIVE
	cos_theta = dot(view_dir, light_dir);
	vec3 B_scAir =  (3.f / (16.f * pi) * beta_R * (1.f + cos_theta * cos_theta));
	vec3 B_scHaze = (4.f * pi) * beta_M * ((pow(1.0 - uG, 2.f) / (pow(1.f + uG*uG - 2.f * uG * cos_theta, 1.5))));
	float s = view_dist * 30.f;
	F_ex = exp(-(beta_R)*s); 
	vec3 L_in = (B_scAir) / (beta_R);
	L_in *= E_sun;
	L_in *= (1.f - F_ex);
	color *= F_ex;
	color += L_in;	

	color = ACESFilm(color);
	color = pow(color, vec3(2.2));

	// Earth shadow
	cos_theta = dot(normalize(sun_position - uCameraPosition), vec3(0.f, 1.f, 0.f));
	if (cos_theta <= 0.f)
		color *= exp(-abs(cos_theta) * 5.f);

	gl_FragColor = vec4(color, 1.0);
}