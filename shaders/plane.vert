#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;
layout (location = 3) in vec3 vertexColor;

uniform mat4 uProj;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjSun;
uniform mat4 uViewSun;

out vec3 fragColor;
out vec3 fragPosition;
out vec4 fragPositionLightSpace;

void main()
{
	gl_Position = uProj * uView * uModel * vec4(vertexPosition, 1.0);
	fragColor = vertexColor;
	fragPosition = (uModel * vec4(vertexPosition, 1.0)).rgb;
	fragPositionLightSpace = uProjSun * uViewSun * vec4(fragPosition, 1.0);
}