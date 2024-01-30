#version 330 core

in vec3 fragPosition;
in vec3 fragColor;
in vec2 fragTexCoord;

uniform sampler2D texture1;
uniform int uFrames;
uniform vec3 uCameraPosition;

const vec3 sun_position = vec3(0.0, 10000.0, 0.0);
const vec3 beta_R = vec3(0.0000037463718226553836, 0.00001, 0.00001573519360000001);
const float beta_M = 0.00001;
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


vec4 toRGBE(vec3 c) {
	vec3 c_abs = abs(c);
	float x = max(c_abs.r, max(c_abs.g, c_abs.b));
	float y = ceil(log2(x));
	return x <= 0.0 ? vec4(0) : vec4(c / exp2(y), (y + 128.0) / 255.0);
}

vec3 rotateVectorAroundOrigin(vec3 vector, float angleInRadians) {
    mat3 rotationMatrix = mat3(
        cos(angleInRadians), 0.0, sin(angleInRadians),
        0.0, 1.0, 0.0,
        -sin(angleInRadians), 0.0, cos(angleInRadians)
    );

    return rotationMatrix * vector;
}

void main()
{
    float angle = uFrames;
    //vec3 rotatedSunPosition = rotateVectorAroundOrigin(sun_position, angle);
    vec3 rotatedSunPosition = sun_position;

	vec3 view_dir = normalize(fragPosition - uCameraPosition);
	vec3 light_dir = normalize(rotatedSunPosition - uCameraPosition);
  	float cos_theta = dot(view_dir, light_dir);
	
	float view_dist = length(fragPosition - uCameraPosition);
	float sun_dist = length(rotatedSunPosition - uCameraPosition);

	// phase function
	const float pi = 3.14159265;
	float Phi_R = 3.0 / (16.0 * pi) * (1.0 + cos_theta * cos_theta);
	float Phi_M = 1.0 / (4.0 * pi) * pow(1.0 - g, 2.0) / pow(1.0 + g * g - 2.0 * g * cos_theta, 1.5);

	vec3 color = vec3(0.0, 0.0, 0.0);

	vec3 F_ex = exp(-(beta_R+beta_M) * view_dist);
	vec3 L_in = ((beta_R * Phi_R + beta_M * Phi_M)/(beta_R + beta_M));
	L_in *= (1.0 - F_ex);
	L_in *= E_sun;
	
	// Normalisez votre intensité
	float maxIntensity = max(L_in.r, max(L_in.g, L_in.b));
	L_in /= maxIntensity;

	color += L_in;
	
	// color = toRGBE(color).rgb;




	/* Noise */
	vec2 pos = vec2(fragTexCoord.x * noise_res, fragTexCoord.y * noise_res) * 5.f;
	float value = composition(pos);
    //value = texture(texture1, fragTexCoord).r;
	value = smoothstep(0.8, 1.3, value);
	float alpha = max(value, 0.2);
	//gl_FragColor = vec4(mix((0.16, 0.32, 0.75), vec3(value), alpha), 1.);
	gl_FragColor = vec4(mix(color, vec3(value), value), 1.);
}
