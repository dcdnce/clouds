#version 330 core

uniform sampler2D texture1;
in vec2 fragTexCoord;

float nearPlane = 0.1;
float farPlane = 10000.0;

void main()
{
	float value = texture(texture1, fragTexCoord).r;
	
	gl_FragColor = vec4(vec3(value), 1.0);
}