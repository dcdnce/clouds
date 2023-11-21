#version 330 core

in vec2 fragTexCoord;
flat in vec3 fragColor;

uniform bool uGreyShading;

void main()
{
    gl_FragColor = vec4(fragColor, 1.0);

    if (uGreyShading)
    {
        float grey = (0.2125 * fragColor.x + 0.1154 * fragColor.y + 0.021 * fragColor.z);    
        gl_FragColor = vec4(grey, grey, grey, 1.0);
    }
}
