#version 330 core

in vec3 fragColor;
in vec2 fragTexCoord;

uniform sampler2D texture1;

void main()
{
    float value = texture(texture1, fragTexCoord).r;
	value = smoothstep(0.5, 0.8, value);
	vec3 color = vec3(value, value, value);
	gl_FragColor = vec4(color.r, color.g, color.b, color.r);
}
