#version 330 core

in vec3 fragPosition;
in vec3 fragColor;
in vec2 fragTexCoord;

uniform sampler2D texture1;
uniform int uFrames;
uniform vec3 uCameraPosition;
uniform vec3 uSunPosition;
uniform mat4 uRotatedSun;
uniform float uCloudsSmoothstepEdgeMin;
uniform float uCloudsSmoothstepEdgeMax;
uniform float uZenith;
uniform float uOpticalLengthAir;
uniform float uOpticalLengthHaze;
uniform int uCloudsRender;
uniform int uAverageDensity;
uniform float uAverageDensityStepSize;
uniform float uNoiseScale;

vec3 beta_R = vec3(6.95e-2, 1.18e-1, 2.44e-1); // Beta Rayleigh - out-scattering already computed coefficients
vec3 beta_M = vec3(2e-4, 2e-4, 2e-4); // Beta Mie - out-scattering already computed coefficients

const float g = 0.95;
vec3 E_sun = vec3(255.0, 255.0, 255.0);
const float pi = 3.14159265;

const float noise_res = 256.f;
float offsets[8];

// 2D Random
float random(vec2 v)
{
    return fract(sin(dot(v.xy,
                         vec2(14.9898,78.233)))
                 * 403758.5453123);
}

float noise(float step, vec2 v)
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

vec4 fbm(vec2 v, vec2 s, const float speed)
{
	vec2 dir = normalize(s - v);

    offsets[0] = -uFrames * 0.1 * speed;
    offsets[1] = -uFrames * 0.02 * speed;
    offsets[2] = uFrames * 0.05 * speed;
    offsets[3] = -uFrames * 0.1 * speed;
    offsets[4] = uFrames * 0.02 * speed;
    offsets[5] = uFrames * 0.03 * speed;
    offsets[6] = uFrames * 0.02 * speed;
    offsets[7] = uFrames * 0.3 * speed;
    float sum[4];
    float sum_weights = 0;

	for (int i = 0 ; i < (bool(uAverageDensity) ? 3 : 1) ; i++) {
		sum[i] = 0;
		if (i != 0)
			v += dir * uAverageDensityStepSize;
		// k = 0 is too much granularity at high scale
		for (int k = 3  ; k < 8 ; k++) { // octaves
			float weight = float(1 << k);
			sum[i] += noise(weight, vec2(v.x + offsets[k], v.y)) * weight;
		}
    	sum[i] += 128.f;
    	sum[i] /= 256.f;
	}
	return vec4(sum[0], sum[1], sum[2], sum[3]);
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
	vec3 sky_rgb = vec3(0.0, 0.0, 0.0);
	vec3 light_dir = normalize(sun_position - uCameraPosition);
	float sun_dist = length(sun_position - uCameraPosition);
	vec3 view_dir = normalize(fragPosition - uCameraPosition);
	float view_dist = length(fragPosition - uCameraPosition);

	// phases functions - in-scattering probability
	float cos_theta = dot(view_dir, light_dir);
	vec3 Phi_R = 3.0 / (16.0 * pi) * beta_R * (1.0 + cos_theta * cos_theta);
	vec3 Phi_M = 1.0 / (4.0 * pi) * beta_M * pow(1.0 - g, 2.0) / pow(1.0 + g * g - 2.0 * g * cos_theta, 1.5);

	// SUN
	// clamped to [0;1]: because normally [-1;1] for opposed, orthogonal, colinear 
	cos_theta = (dot(view_dir, light_dir) + 1.f) * 0.5f;
	float theta = acos(cos_theta);
	float theta_degree = theta * 180.f / pi;
	float air_mass = 1.0 / (theta_degree + 0.15 * pow(93.885 - theta_degree, -1.253));
	float sA = 2.f * air_mass; // otherwise sun is too small
	float sH = uOpticalLengthHaze  * air_mass;
	vec3 F_ex = exp(-(beta_R*sA+beta_M*sH));
	vec3 L_in = (Phi_R + Phi_M) / (beta_R + beta_M);
	L_in *= E_sun;
	L_in *= (1.0 - F_ex);
	sky_rgb += L_in;

	// AERIAL PERSPECTIVE
	sA = view_dist * uOpticalLengthAir / uZenith;
	sH = view_dist * uOpticalLengthHaze / uZenith; // doesnt change depending on sun angle
	F_ex = exp(-(beta_R*sA+beta_M*sH));
	L_in = (Phi_R + Phi_M) / (beta_R + beta_M);
	L_in *= E_sun;
	L_in *= (1.0 - F_ex);
	sky_rgb *= F_ex;
	sky_rgb += L_in;

	// aesthetic
	sky_rgb = ACESFilm(sky_rgb);
	sky_rgb = pow(sky_rgb, vec3(2.2));

	/* CLOUDS */
	// Cumulus
	vec2 pos = vec2(fragTexCoord.x * noise_res, fragTexCoord.y * noise_res) * uNoiseScale;
	vec4 cumulus = fbm(pos, vec2(sun_position.x, sun_position.z), 0.3);
	cumulus.x = smoothstep(uCloudsSmoothstepEdgeMin, uCloudsSmoothstepEdgeMax, cumulus.x); // cumulus like
	float cumulus_alpha = cumulus.x; // keep alpha value before applying average density !
	vec3 cumulus_rgb = vec3(cumulus.x);

	// AERIAL PERSPECTIVE on clouds - looks better
	cumulus_rgb *= F_ex;
	cumulus_rgb += L_in;
	cumulus_rgb = ACESFilm(cumulus_rgb);

	// Average density
	if (bool(uAverageDensity)) {
		float average_density = (cumulus.x + cumulus.y + cumulus.z) / 3.f; 
		average_density = mix(1.0, 0.0, average_density);
		vec3 smoothed_density = smoothstep(-0.8, 0.2, vec3(average_density));
    	cumulus_rgb *= smoothed_density;
	}

	// Final Color
	vec4 tot_rgb = vec4(sky_rgb, 1.);

	if (bool(uCloudsRender)) {
		tot_rgb = vec4(mix(sky_rgb, cumulus_rgb, cumulus_alpha), 1.);
	}

	if (light_dir.y < 0.0) // earth shadow
		tot_rgb *= exp(-10.0 * -light_dir.y);
	if (view_dir.y < 0.0) // under earth
		tot_rgb *= mix(1., 0., (view_dir.y * -1) * 5.f);
	gl_FragColor = tot_rgb;
}
