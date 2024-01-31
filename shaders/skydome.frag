#version 330 core

in vec3 fragPosition;
in vec3 fragColor;
in vec2 fragTexCoord;

uniform sampler2D texture1;
uniform int uFrames;
uniform vec3 uCameraPosition;
uniform mat4 uRotatedSun;

vec3 sun_position = vec3(-11.0, 11.0, 0.0);
vec3 beta_R = vec3(6.95e-2, 1.18e-1, 2.44e-1);
vec3 beta_M = vec3(4e-2, 4e-2, 4e-2);
const float g = 0.9;
const vec3 E_sun = vec3(250.0, 235.0, 200.0);

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
	// Four corners
	float a = random(i);
	float b = random(i + vec2(step, 0));
	float c = random(i + vec2(0, step));
	float d = random(i + vec2(step, step));
	float xf = f.x;
	float yf = f.y;
	// Cubic Hermine Curve
	xf = xf*xf*(3.f-2.f*xf);
	yf = yf*yf*(3.f-2.f*yf);
	// Mix - interpolation
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
	// CLASSIC SCATTERING
    sun_position = vec3(vec4(uRotatedSun * vec4(sun_position, 1.0)).rgb);
	vec3 view_dir = normalize(fragPosition - uCameraPosition);
	vec3 light_dir = normalize(sun_position - uCameraPosition);
  	float cos_theta = clamp(dot(view_dir, light_dir), 0.0, 1.0);
	float view_dist = length(fragPosition - uCameraPosition);
	float sun_dist = length(sun_position - uCameraPosition);
	vec3 sky_rgb = vec3(0.0, 0.0, 0.0);
	// optical depth -> zenithAngle
	float t = max(0.001, view_dir.y) + max(-view_dir.y, -0.001);
	float sR = 1. / t ;
	float sM = 1.5 / t ;
	// phases functions
	const float pi = 3.14159265;
	float Phi_R = 3.0 / (16.0 * pi) * (1.0 + cos_theta * cos_theta);
	float Phi_M = 1.0 / (4.0 * pi) * pow(1.0 - g, 2.0) / pow(1.0 + g * g - 2.0 * g * cos_theta, 1.5);
	// coefficients
	vec3 F_ex = exp(-(beta_R*sR+beta_M*sM));
	// vec3 F_ex = exp(-(beta_R+beta_M) * view_dist);
	vec3 L_in = ((beta_R * Phi_R + beta_M * Phi_M)/(beta_R + beta_M));
	L_in *= (1.0 - F_ex);
	L_in *= E_sun;
	L_in *= 0.08; // kSunIntensity
	sky_rgb += L_in;
 	// sun
	sky_rgb += 0.47*vec3(1.6,1.4,1.0)*pow(cos_theta, 350.0 ) * F_ex;
	// sun haze
	sky_rgb += 0.4*vec3(0.8,0.9,1.0)*pow(cos_theta, 2.0 )* F_ex;
	sky_rgb = ACESFilm(sky_rgb);
	sky_rgb = pow(sky_rgb, vec3(2.2));


	/* NOISE */
	vec2 pos = vec2(fragTexCoord.x * noise_res, fragTexCoord.y * noise_res) * 5.f;
	float cloud = composition(pos);
	cloud = smoothstep(0.8, 1.3, cloud);

	// SUNLIGHT on clouds
	vec3 light_dir_from_center = normalize(sun_position - vec3(0.0, 0.0, 0.0));
	vec3 view_dir_from_center = normalize(fragPosition - vec3(0.0, 0.0, 0.0));
	float cos_theta_degrees = clamp(dot(view_dir_from_center, light_dir_from_center), -1.0, 1.0);
	cos_theta_degrees = max(0.0, cos_theta_degrees); // no more sunlight
	float theta_degrees = acos(cos_theta_degrees);
	float zenith = 2.0;
	float s = zenith / (cos_theta_degrees + 0.15 * pow(93.885 - theta_degrees, -1.253));
	F_ex = exp(-(beta_R+beta_M) * s);
	vec3 cloud_rgb = vec3(cloud);
	cloud_rgb *= F_ex;
	cloud_rgb += L_in;
	cloud_rgb *= 0.6f; //constant

	// float alpha = max(cloud, 0.2);
	gl_FragColor = vec4(mix(sky_rgb, cloud_rgb, cloud), 1.);
}
