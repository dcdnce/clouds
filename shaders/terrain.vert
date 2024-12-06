#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in vec3 inColor;

uniform mat4 uProj;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjSun;
uniform mat4 uViewSun;

out vec3 vertexColor;
out vec3 vertexPosition;
out vec4 fragPositionLightSpace;
out vec3 fragNormal;

void main()
{
	gl_Position = uProj * uView * uModel * vec4(inPosition, 1.0);
	vertexColor = inColor;
	vertexPosition = (uModel * vec4(inPosition, 1.0)).rgb;
	fragPositionLightSpace = uProjSun * uViewSun * vec4(vertexPosition, 1.0);
	fragNormal = inNormal;
}