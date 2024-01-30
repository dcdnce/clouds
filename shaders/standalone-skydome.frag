// Author:
// Title:

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

const float noise_res = 256.0;
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
        float a = random(i);
        float b = random(i + vec2(step, 0));
        float c = random(i + vec2(0, step));
        float d = random(i + vec2(step, step));
        float xf = f.x;
        float yf = f.y;
        xf = xf*xf*(3.-2.*xf);
        yf = yf*yf*(3.-2.*yf);
        float nx0 = a*(1.-xf)+b*xf;
        float nx1 = c*(1.-xf)+d*xf;
        return(nx0*(1.-yf)+nx1*yf);
}

float composition(vec2 v)
{
    offsets[0] = -u_time * 0.1   * 10.;
    offsets[1] = -u_time * 0.02 * 10.;
    offsets[2] = u_time * 0.05 * 10.;
    offsets[3] = -u_time * 0.1  * 10.;
    offsets[4] = u_time * 0.02   * 10.;
    offsets[5] = u_time * 0.03   * 10.;
    offsets[6] = u_time * 0.02  * 100.;
    offsets[7] = u_time * 0.2 * 100.;
    
    float sum = 0.;
    float sum_weights = 0.;

    for (int k = 0  ; k < 8; k++) { // octave
        float weight = pow(2.0, float(k));
        sum += interpolate(weight, vec2(v.x + offsets[k], v.y)) * weight;
        sum_weights += weight;
    }

    sum += 128.;
    return sum / sum_weights;
}


void main() {
    float value = composition(gl_FragCoord.xy * 1.5);

	value = smoothstep(0.8, 1.3, value);
    float alpha = max(value, 0.20);
    gl_FragColor = vec4(mix(vec3(0.16, 0.32, 0.75), vec3(value), alpha), 1.);
}