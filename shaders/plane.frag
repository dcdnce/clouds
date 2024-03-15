#version 330 core

uniform sampler2D texture_depth;
uniform vec3 uSunPosition;
uniform mat4 uRotatedSun;
uniform int uFrames;
uniform vec3 uCameraPosition;
uniform float uOpticalLengthAir;
uniform float uOpticalLengthHaze;

in vec3 fragPosition;
in vec3 fragColor;
in vec4 fragPositionLightSpace;

vec3 beta_R = vec3(6.95e-2, 1.18e-1, 2.44e-1);
vec3 beta_M = vec3(2e-4, 2e-4, 2e-4);
const float g = 0.95;
vec3 E_sun = vec3(250.0, 235.0, 200.0);

float CloudsShadowScalar()
{
	// perform perspective divide
	vec3 projCoords = fragPositionLightSpace.xyz / fragPositionLightSpace.w; //[-1;1]
	projCoords = projCoords * 0.5 + 0.5; //[0;1]
	// texture saved depth
	float textureDepth = texture(texture_depth, projCoords.xy).r;
	// current fragment depth
	float currentDepth = projCoords.z;

	float shadow = currentDepth < textureDepth ? 1.0 : textureDepth;
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
	// vec3 color = vec3(0.0, 0.0, 0.0);
	vec3 light_dir = normalize(sun_position - uCameraPosition);
	vec3 view_dir = normalize(fragPosition - uCameraPosition);
	float view_dist = length(fragPosition - uCameraPosition);
	float cos_theta = (dot(view_dir, light_dir) + 1.) / 2.; // full length
	const float pi = 3.14159265;
	float theta_degree = acos(cos_theta) * 180.f / pi;
	// phases functions
	float Phi_R = 3.0 / (16.0 * pi) * (1.0 + cos_theta * cos_theta);
	float Phi_M = 1.0 / (4.0 * pi) * pow(1.0 - g, 2.0) / pow(1.0 + g * g - 2.0 * g * cos_theta, 1.5);


	// Diffuse
	vec3 normal = vec3(0.0, 1.0, 0.0);
	vec3 ambient = 0.9 * (E_sun/255.0);
	vec3 diffuse = (E_sun/255.0) * dot(normal, normalize(sun_position - vec3(0.0, 6000.0, 0.0)));
	float shadow = CloudsShadowScalar();
	color *= ambient + shadow * diffuse;    

	// AERIAL PERSPECTIVE
	float sA = view_dist * uOpticalLengthAir / 40000.f; // fucking constant
	vec3 F_ex = exp(-(beta_R*sA));
	vec3 L_in = (beta_R * Phi_R) / (beta_R);
	L_in *= (1.0 - F_ex);
	L_in *= E_sun;
	color *= F_ex;
	color += L_in;
	color = ACESFilm(color);
	color = pow(color, vec3(2.2));

	gl_FragColor = vec4(color, 1.0);
}