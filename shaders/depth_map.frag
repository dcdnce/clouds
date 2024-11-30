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
	// gl_FragDepth = 1.0;
	if (fragPosition.y < 6000.0)
		discard;

    vec3 sun_position = vec3(vec4(uRotatedSun * vec4(uSunPosition, 1.0)).rgb);
	/* CLOUDS */
	vec2 pos = vec2(fragTexCoord.x * noise_res, fragTexCoord.y * noise_res) * uNoiseScale;
	vec4 cumulus = fbm(pos, vec2(sun_position.x, sun_position.z), 0.3);
	cumulus.x = smoothstep(uCloudsSmoothstepEdgeMin, uCloudsSmoothstepEdgeMax, cumulus.x); // cumulus like
	if (cumulus.x < 0.9) {
		gl_FragDepth = 1.0;
	}
	else {
		// gl_FragDepth = 1.0 - cumulus.x;
		gl_FragDepth = 1.0 - mix(0.0, 1.0, (cumulus.x - 0.9)/ (1.2 - 0.9));
	}
}
