#version 330 core

in vec3 fragColor;
in vec2 fragTexCoord;

uniform sampler2D texture1;

// 2D Random
float random(vec2 v)
{
    return fract(sin(dot(v.xy,
                         vec2(12.9898,78.233)))
                 * 43758.5453123);
}

float interpolate(int step, vec2 v)
{
	vec2 i = floor(v);
    vec2 f = vec2(fract(v.x), fract(v.y));

	// Four corners
	float a = random(vec2(i.x, i.y));
	float b = random(vec2(mod(i.x+float(step), 255.0), i.y));
	float c = random(vec2(i.x, mod(i.y+float(step), 255.0)));
	float d = random(vec2(mod(i.x+float(step), 255.0), mod(i.y+float(step), 255.0)));

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

int composition(vec2 v)
{
	//composition
	float sum = 0;
	for (int k = 1 ; k < 8 ; k++) { // octaves
		vec2 new_v = v;
		sum += interpolate(1<<k, new_v) * (1<<k);
	}
	sum += 128; 
	return(int(sum) >> 8);
}

void main()
{
	vec2 pos = vec2(fragTexCoord.x * 10, fragTexCoord.y * 10);
	float value = composition(pos);

    // float value = texture(texture1, fragTexCoord).r;
	// value = smoothstep(0.5, 0.8, value);
	vec3 color = vec3(value, value, value);
	gl_FragColor = vec4(color.r, color.g, color.b, color.r);
}
