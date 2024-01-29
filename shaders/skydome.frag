#version 330 core

in vec3 fragColor;
in vec2 fragTexCoord;

uniform sampler2D texture1;
uniform int uFrames;

float offsets[8];

// 2D Random
float random(vec2 v)
{
    return fract(sin(dot(v.xy,
                         vec2(12.9898,78.233)))
                 * 43758.5453123);
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
    offsets[0] = uFrames * 0.055;
    offsets[1] = uFrames * 0.060;
    offsets[2] = uFrames * 0.065;
    offsets[3] = uFrames * 0.080;
    offsets[4] = uFrames * 0.085;
    offsets[5] = uFrames * 0.1;
    offsets[6] = uFrames * 0.15;
    offsets[7] = uFrames * 0.16;

    float sum = 0;
    float sum_weights = 0;

    for (int k = 1 ; k <= 8 ; k++) { // octaves
        float weight = float(1 << k);
        sum += interpolate(weight, vec2(v.x + (fract(offsets[k-1] / 256.f) * 256.f), v.y + floor(offsets[k-1] / 256.f))) * weight;
        sum_weights += weight;
    }

    sum += 128.f;
    return sum / sum_weights;
}

void main()
{
	vec2 pos = vec2(fragTexCoord.x * 256, fragTexCoord.y * 256);
	float value = composition(pos);

    //value = texture(texture1, fragTexCoord).r;
	value = smoothstep(0.5, 0.8, value);
	vec3 color = vec3(value, value, value);
	gl_FragColor = vec4(color.r, color.g, color.b, color.r);
}
