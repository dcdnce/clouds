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
uniform float uZenithalOpticalLengthAir;
uniform float uZenithalOpticalLengthHaze;
uniform int uCloudsRender;
uniform int uAverageDensity;
uniform float uAverageDensityStepSize;
uniform float uNoiseScale;

vec3 beta_R = vec3(6.95e-6, 1.18e-5, 2.44e-5); // Beta Rayleigh - out-scattering already computed coefficients
vec3 beta_M = vec3(2e-5, 3e-5, 4e-5); // Beta Mie - out-scattering already computed coefficients

const float g = -1;
vec3 E_sun = vec3(255.0, 255.0, 255.0);
// vec3 E_sun = vec3(1.f, 1.f, 1.f);
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
	vec3 zenith_dir = normalize(vec3(0.f, uZenith, 0.f) - uCameraPosition);
	float sun_dist = length(sun_position - uCameraPosition);
	vec3 view_dir = normalize(fragPosition - uCameraPosition);
	float view_dist = length(fragPosition - uCameraPosition);

	// SUNLIGHT
	float cos_theta = clamp(dot(view_dir, zenith_dir), 0.f, 1.f);
	float theta_degree = acos(cos_theta) * 180.f / pi;
	float air_mass = 1.0 / (cos_theta + 0.15 * pow(93.885 - theta_degree, -1.253));
	float sAir = uZenithalOpticalLengthAir * air_mass;
	float sHaze = uZenithalOpticalLengthHaze * air_mass;
	vec3 F_ex = exp(-(beta_R*sAir+beta_M*sHaze));
	E_sun *= F_ex;

	// GENERAL EQUATION - AERIAL PERSPECTIVE
	cos_theta = clamp(dot(view_dir, light_dir), 0.f, 1.f);
	// Scattering coefficients (quantity) multiplied by their phase function (angular direction)
	vec3 B_scAir =  (3.f / (16.f * pi) * beta_R * (1.f + cos_theta * cos_theta));
	vec3 B_scHaze = (4.f * pi) * beta_M * ((pow(1.0 - g, 2.f) / (pow(1.f + g*g - 2.f * g * cos_theta, 1.5))));

	sAir = view_dist / uZenith;
	sHaze = view_dist / uZenith;

	F_ex = exp(-((beta_R + beta_M)*sAir));
	vec3 L_in = (B_scAir + B_scHaze) / (beta_R + beta_M);
	L_in *= E_sun;
	L_in *= 1.f - F_ex;
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
	// Average density
	float average_density = (cumulus.x + cumulus.y + cumulus.z) / 3.f; 
	average_density = mix(1.0, 0.0, average_density);
	vec3 smoothed_density = smoothstep(-0.8, 0.2, vec3(average_density));

	// cumulus_rgb = ACESFilm(cumulus_rgb);

	// Average density bool
	if (bool(uAverageDensity)) {
    	cumulus_rgb *= smoothed_density;
	}

	// Final Color
	vec4 tot_rgb = vec4(sky_rgb, 1.);

	if (bool(uCloudsRender)) {
		tot_rgb = vec4(mix(sky_rgb, cumulus_rgb, cumulus_alpha), 1.);
	}

	if (light_dir.y < 0.0) // earth shadow
		tot_rgb *= exp(-1.0 * -light_dir.y);
	if (view_dir.y < 0.0) // under earth
		tot_rgb *= mix(1., 0., (view_dir.y * -1) * 5.f);
	gl_FragColor = tot_rgb;
}
