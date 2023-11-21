#version 330 core

in vec2 fragTexCoord;
flat in vec3 fragColor;

uniform sampler2D texture1;

void main()
{
    gl_FragColor = texture(texture1, fragTexCoord);
}
