#version 330 core
layout (location = 0) in vec2 vertexPosition;
layout (location = 1) in vec2 vertexTexCoord;

out vec2 fragTexCoord;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
    gl_Position = uProj * uView * uModel * vec4(vertexPosition, 0.0, 1.0);
    // gl_Position = vec4(vertexPosition, 0.0, 1.0);
    fragTexCoord = vertexTexCoord;
}
