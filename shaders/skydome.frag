#version 330 core

in vec3 fragColor;
in vec2 fragTexCoord;

uniform sampler2D texture1;
uniform int uFrames;

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
    offsets[0] = uFrames * 0.2   * 0.5f;
    offsets[1] = uFrames * 0.060 * 0.5f;
    offsets[2] = uFrames * 0.065 * 0.5f;
    offsets[3] = uFrames * 0.25  * 0.5f;
    offsets[4] = uFrames * 0.3   * 0.5f;
    offsets[5] = uFrames * 0.1   * 0.5f;
    offsets[6] = uFrames * 0.15  * 0.5f;
    offsets[7] = uFrames * 0.26  * 0.5f;

    float sum = 0;
    float sum_weights = 0;

    for (int k = 0  ; k < 8 ; k++) { // octaves
        float weight = float(1 << k);
        sum += interpolate(weight, vec2(v.x + (fract(offsets[k] / noise_res) * noise_res), v.y + floor(offsets[k] / noise_res))) * weight;
        sum_weights += weight;
    }

    sum += 128.f;
    return sum / 256.f;
}

void main()
{
	vec2 pos = vec2(fragTexCoord.x * noise_res, fragTexCoord.y * noise_res);
	float value = composition(pos);

    //value = texture(texture1, fragTexCoord).r;
	value = smoothstep(0.5, 1.3, value);
	vec3 color = vec3(value, value, value);
	float alpha = max(value, 0.2);
	gl_FragColor = vec4(color.r, color.g, color.b, alpha);
}
