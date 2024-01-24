#version 330 core

in vec3 fragColor;
in vec2 fragTexCoord;

uniform sampler2D texture1;

void main()
{
    float value = texture(texture1, fragTexCoord).r;
    vec3 color = vec3(value, value, value);

	gl_FragColor = vec4(1.0 - color.r, 1.0 - color.g, 1.0 - color.b, 1.0 - color.r);
}
