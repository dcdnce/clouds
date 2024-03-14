#version 330 core

smooth in vec2 geomTexCoord;
smooth in vec3 geomWorldPos;
smooth in vec4 geomEyeSpacePos;

void main()
{
	gl_FragColor = vec4(vec3(1.0), 1.0);
}