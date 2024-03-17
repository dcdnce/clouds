#version 330 core

smooth in vec2 geomTexCoord;
smooth in vec3 geomWorldPos;
smooth in vec4 geomEyeSpacePos;

uniform sampler2D texture_grass;

void main()
{
	vec4 color = texture(texture_grass, vec2(geomTexCoord.x, 1.0 - geomTexCoord.y));
	color.a = min(1.0, color.a * 1.25);
	if (color.a < 0.25)
		discard ;
	gl_FragColor = color;
}