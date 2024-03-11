#version 330 core

layout (location = 0) in vec3 vertexPosition;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
	gl_Position = uProj * uView * uModel * vec4(vertexPosition, 1.0);
}