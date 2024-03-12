#version 330 core

in vec3 fragPosition;
in vec3 fragColor;
in vec2 fragTexCoord;

uniform float uCloudsSmoothstepEdgeMin;
uniform float uCloudsSmoothstepEdgeMax;
uniform float uNoiseScale;
uniform int uAverageDensity;
uniform float uAverageDensityStepSize;
uniform int uFrames;
uniform mat4 uRotatedSun;
uniform vec3 uSunPosition;

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
		sum_weights = 0;
		if (i != 0)
			v += dir * uAverageDensityStepSize;
		// k = 0 is too much granularity at high scale
		for (int k = 3  ; k < 8 ; k++) { // octaves
			float weight = float(1 << k);
			sum[i] += noise(weight, vec2(v.x + offsets[k], v.y)) * weight;
			sum_weights += weight;
		}
    	sum[i] += 128.f;
    	sum[i] /= 256.f;
	}
	return vec4(sum[0], sum[1], sum[2], sum[3]);
}



void main()
{
    // vec3 sun_position = vec3(vec4(uRotatedSun * vec4(uSunPosition, 1.0)).rgb);
	// vec3 sky_rgb = vec3(0.0, 0.0, 0.0);

	// /* CLOUDS */
	// // Cumulus
	// vec2 pos = vec2(fragTexCoord.x * noise_res, fragTexCoord.y * noise_res) * uNoiseScale;
	// vec4 cumulus = fbm(pos, vec2(sun_position.x, sun_position.z), 0.3);
	// cumulus.x = smoothstep(uCloudsSmoothstepEdgeMin, uCloudsSmoothstepEdgeMax, cumulus.x); // cumulus like
	// float cumulus_alpha = cumulus.x; // keep alpha value before applying average density !
	// // if (bool(uAverageDensity)) {
	// // 	float average_density = (cumulus.x + cumulus.y + cumulus.z) / 3.f; 
	// // 	average_density = mix(1.0, 0.0, average_density);
	// // 	cumulus.x = smoothstep(-0.8, 0.3, average_density);
	// // }
	// gl_FragDepth = 0.5;
}

