#version 330 core

in vec2 fragTexCoord;

uniform sampler2D texture1;

void main()
{
    float value = texture(texture1, fragTexCoord).r;
    gl_FragColor = vec4(value, value, value, 1.0);
}
