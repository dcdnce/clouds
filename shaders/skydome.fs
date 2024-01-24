#version 330 core

in vec3 fragColor;
in vec2 fragTexCoord;

uniform sampler2D texture1;

void main() {
	// gl_FragColor = vec4(fragColor, 1.);
    float value = texture(texture1, fragTexCoord).r;
    vec3 color = vec3(value, value, value);

	if (value > 0.9) {
		gl_FragColor = vec4(color, 0.0);
	}
	else {
		gl_FragColor = vec4(1.0 - color.r, 1.0 - color.g, 1.0 - color.b, 1.0);
		//gl_FragColor = vec4(color, 1.0);
	}
}
