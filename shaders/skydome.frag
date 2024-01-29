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

float interpolate(float step, vec2 v)
{
	//vec2 i = floor(v);
    //vec2 f = vec2(fract(v.x), fract(v.y));
	float step_sq = step*step;
	float index = v.y * 256.0 + v.x;
	float integer = floor(index / step) * step; // a
	float frac = fract(index / step) * step;

	vec2 i = vec2(fract(integer/ 256.0) * 256.0, floor(integer / 256.0));
	vec2 f = vec2(fract(frac / 256.0)* 256.0, floor(frac / 256.0));


	// Four corners
	float a = random(vec2(i.x, i.y));
	float b = random(vec2(i.x+step, i.y));
	float c = random(vec2(i.x, i.y+step));
	float d = random(vec2(i.x+step, i.y+step));

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
	//composition
	float sum = 0;
	for (int k = 8 ; k <= 8 ; k++) { // octaves
		vec2 new_v = v;
		//sum += interpolate(float(1<<k), new_v) * (1<<k);
		sum += interpolate(16.f, new_v) * 1.f;
	}
	sum += 128; 
	return(sum / 256);
}

void main()
{
	vec2 pos = vec2(fragTexCoord.x * 256, fragTexCoord.y * 256);
	float value = composition(pos);

    // float value = texture(texture1, fragTexCoord).r;
	// value = smoothstep(0.5, 0.8, value);
	vec3 color = vec3(value, value, value);
	gl_FragColor = vec4(color.r, color.g, color.b, color.r);
}
