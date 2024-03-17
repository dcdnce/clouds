#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 12) out;

uniform mat4 uProj;
uniform mat4 uModel;
uniform mat4 uView;
uniform float uFrames;

smooth out vec2 geomTexCoord;
smooth out vec3 geomWorldPos;
smooth out vec4 geomEyeSpacePos;

vec3 local_seed = vec3(0.0);

mat4 RotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

// This function returns random number from zero to one
float RandZeroOne()
{
    uint n = floatBitsToUint(local_seed.y * 214013.0 + local_seed.x * 2531011.0 + local_seed.z * 141251.0);
    n = n * (n * n * 15731u + 789221u);
    n = (n >> 9u) | 0x3F800000u;
 
    float fRes =  2.0 - uintBitsToFloat(n);
    local_seed = vec3(local_seed.x + 147158.0 * fRes, local_seed.y*fRes  + 415161.0 * fRes, local_seed.z + 324154.0*fRes);
    return fRes;
}

int RandomInt(int min, int max)
{
	float fRandomFloat = RandZeroOne();
	return int(float(min)+fRandomFloat*float(max-min));
}

void main()
{
	mat4 mvp = uProj * uView * uModel;
	vec3 grass_field_pos = gl_in[0].gl_Position.xyz;

	float piover180 = 3.1415/180.0;
	vec3 base_dirs[3];
	base_dirs[0] = vec3(1.0, 0.0, 0.0);
	base_dirs[1] = vec3(float(cos(45.0*piover180)), 0.0f, float(sin(45.0*piover180)));
	base_dirs[2] = vec3(float(cos(-45.0*piover180)), 0.0f, float(sin(-45.0*piover180)));
	float grass_patch_size = 5.0;

	for (int i = 0 ; i < 3 ; i++) {
		vec3 base_dir_rotated = (RotationMatrix(vec3(0, 1, 0), sin(uFrames*0.7f)*0.1f)*vec4(base_dirs[i], 1.0)).xyz;
		local_seed = grass_field_pos * float(i);
		float grass_patch_height = 3.5+RandZeroOne()*2.0;
		// Texture Coords
		int grass_patch = RandomInt(0, 3);
		float texcoord_start_x = float(grass_patch)*0.25f;
		float texcoord_end_x = texcoord_start_x+0.25f;

		// top left
		vec3 TL = grass_field_pos - base_dir_rotated * grass_patch_size * 0.5f;
		TL.y += grass_patch_height;
		gl_Position = mvp*vec4(TL, 1.0);
		geomTexCoord = vec2(texcoord_start_x, 1.0);
		geomWorldPos = TL;
		geomEyeSpacePos = uView * uModel * vec4(TL, 1.0);
		EmitVertex();
		// bottom left
		vec3 BL = grass_field_pos - base_dirs[i]*grass_patch_size*0.5f;
		gl_Position = mvp*vec4(BL, 1.0);
		geomTexCoord = vec2(texcoord_start_x, 0.0);
		geomWorldPos = BL;
		geomEyeSpacePos = uView * uModel * vec4(BL, 1.0);
		EmitVertex();
		// top right
		vec3 TR = grass_field_pos + base_dir_rotated*grass_patch_size*0.5f;
		TR.y += grass_patch_height;
		gl_Position = mvp*vec4(TR, 1.0);
		geomTexCoord = vec2(texcoord_end_x, 1.0);
		geomWorldPos = TR;
		geomEyeSpacePos = uView * uModel * vec4(TR, 1.0);
		EmitVertex();
		// bottom right
		vec3 BR = grass_field_pos + base_dirs[i]*grass_patch_size*0.5f;
		gl_Position = mvp*vec4(BR, 1.0);
		geomTexCoord = vec2(texcoord_end_x, 0.0);
		geomWorldPos = BR;
		geomEyeSpacePos = uView * uModel * vec4(BR, 1.0);
		EmitVertex();

		EndPrimitive();
	}
}