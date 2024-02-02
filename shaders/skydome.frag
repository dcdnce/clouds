#version 330 core

in vec3 fragPosition;
in vec3 fragColor;
in vec2 fragTexCoord;

uniform sampler2D texture1;
uniform int uFrames;
uniform vec3 uCameraPosition;
uniform mat4 uRotatedSun;
uniform float uSkydomeRadius;

vec3 sun_position = vec3(0.0, 1000000.0, 0.0);
vec3 beta_R = vec3(6.95e-2, 1.18e-1, 2.44e-1);
vec3 beta_M = vec3(2e-4, 2e-4, 2e-4);
const float g = 0.95;
vec3 E_sun = vec3(250.0, 235.0, 200.0);

const float noise_res = 256.f;
float offsets[8];

// 2D Random
float random(vec2 v)
{
    return fract(sin(dot(v.xy,
                         vec2(14.9898,78.233)))
                 * 403758.5453123);
}

float interpolate(float step, vec2 v)
{
	vec2 i = floor(v / step) * step;
	vec2 f = fract(v / step);
	float a = random(i);
	float b = random(i + vec2(step, 0));
	float c = random(i + vec2(0, step));
	float d = random(i + vec2(step, step));
	float xf = f.x;
	float yf = f.y;
	xf = xf*xf*(3.f-2.f*xf);
	yf = yf*yf*(3.f-2.f*yf);
	float nx0 = a*(1.f-xf)+b*xf;
	float nx1 = c*(1.f-xf)+d*xf;
	return(nx0*(1.f-yf)+nx1*yf);
}

float composition(vec2 v)
{
    offsets[0] = -uFrames * 0.1;
    offsets[1] = -uFrames * 0.02;
    offsets[2] = uFrames * 0.05;
    offsets[3] = -uFrames * 0.1;
    offsets[4] = uFrames * 0.02;
    offsets[5] = uFrames * 0.03;
    offsets[6] = uFrames * 0.02;
    offsets[7] = uFrames * 0.3;
    float sum = 0;
    float sum_weights = 0;

    for (int k = 0  ; k < 8 ; k++) { // octaves
        float weight = float(1 << k);
        sum += interpolate(weight, vec2(v.x + offsets[k], v.y)) * weight;
        sum_weights += weight;
    }
    sum += 128.f;
    return sum / 256.f;
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
    sun_position = vec3(vec4(uRotatedSun * vec4(sun_position, 1.0)).rgb);
	vec3 sky_rgb = vec3(0.0, 0.0, 0.0);
	vec3 light_dir = normalize(sun_position - uCameraPosition);
	vec3 view_dir = normalize(fragPosition - uCameraPosition);
	float view_dist = length(fragPosition - uCameraPosition);
	float cos_theta = (dot(view_dir, light_dir) + 1.) / 2.; // full length
	float theta = acos(cos_theta);
	const float pi = 3.14159265;
	float theta_degree = theta * 180.f / pi;
	// phases functions
	float Phi_R = 3.0 / (16.0 * pi) * (1.0 + cos_theta * cos_theta);
	float Phi_M = 1.0 / (4.0 * pi) * pow(1.0 - g, 2.0) / pow(1.0 + g * g - 2.0 * g * cos_theta, 1.5);

	// SUNLIGHT 
	float sA = 8.4 / (theta_degree + 0.15 * pow(93.885 - theta_degree, -1.253));
	float sH = 1.25 / (theta_degree + 0.15 * pow(93.885 - theta_degree, -1.253));
	vec3 F_ex = exp(-(beta_R*sA+beta_M*sH));
	vec3 L_in = ((beta_R * Phi_R + beta_M * Phi_M)/(beta_R + beta_M));
	L_in *= (1.0 - F_ex);
	L_in *= E_sun;
	sky_rgb += L_in;

	// SKY COLOR - edge gradient
	float zenith = length(uCameraPosition - vec3(0., uSkydomeRadius, 0.));
	sA = view_dist * 8.4 / zenith; // need to compute zenith
	sH = view_dist * 1.25 / zenith;
	F_ex = exp(-(beta_R*sA+beta_M*sH));
	L_in = ((beta_R * Phi_R + beta_M * Phi_M)/(beta_R + beta_M));
	L_in *= 1.f - exp(-(beta_R+beta_M)*sH);
	L_in *= (beta_R * Phi_R) / beta_R;
	L_in *= 1.f - exp(-(beta_R*(sA - sH)));
	L_in *= exp(-(beta_R+beta_M)*sH);
	L_in *= E_sun;
	sky_rgb += L_in;

	// aesthetic
	sky_rgb = ACESFilm(sky_rgb);
	sky_rgb = pow(sky_rgb, vec3(2.2));

	/* CLOUDS */
	vec2 pos = vec2(fragTexCoord.x * noise_res, fragTexCoord.y * noise_res) * 10.f;
	float cloud = composition(pos);
	cloud = smoothstep(0.8, 1.3, cloud);
	// SUNLIGHT on clouds - TEMPORAIRE
	sA = (view_dist * 0.5) * 8.4 / zenith; 
	sH = (view_dist * 0.5) * 1.25 / zenith;
	F_ex = exp(-(beta_R*sA+beta_M*sH));
	E_sun *= F_ex;
	L_in = ((beta_R * Phi_R + beta_M * Phi_M)/(beta_R + beta_M));
	L_in *= (1.0 - F_ex);
	L_in *= E_sun;
	vec3 cloud_rgb = vec3(cloud);
	cloud_rgb *= F_ex;
	cloud_rgb += L_in;

	// Final Color
	vec4 tot_rgb = vec4(mix(sky_rgb, cloud_rgb, cloud), 1.);
	if (light_dir.y < 0.0) // earth shadow
		tot_rgb *= mix(1., 0., light_dir.y * -1);
	gl_FragColor = tot_rgb;
	// gl_FragColor = mix(vec4(0.0), view_rgb, view_dir.y); // test
}
