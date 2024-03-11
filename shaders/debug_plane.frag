#version 330 core

uniform sampler2D texture1;
in vec2 fragTexCoord;

void main()
{
	gl_FragColor = vec4(texture(texture1, fragTexCoord).rrr, 1.0);
	// gl_FragColor = vec4(1.0);
}