#version 330 core

uniform sampler2D texture1;
in vec2 fragTexCoord;

float zNear = 0.1;
float zFar = 10000.0;

float linearize_depth(float depth)
{
	return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
}

void main()
{
	float value = texture(texture1, fragTexCoord).r;
	
	gl_FragColor = vec4(vec3(linearize_depth(value)), 1.0);
	// gl_FragColor = vec4(vec3(value), 1.0);
}