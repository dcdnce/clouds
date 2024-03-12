#version 330 core

uniform sampler2D texture1;
in vec2 fragTexCoord;

void main()
{
	float value = texture(texture1, fragTexCoord).r;
	
	gl_FragColor = vec4(vec3(value), 1.0);
}