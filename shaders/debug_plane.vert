#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

out vec2 fragTexCoord;

void main()
{
	gl_Position = uProj * uView * uModel * vec4(vertexPosition, 1.0);
	fragTexCoord = vertexTexCoord;
}