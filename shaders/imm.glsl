// this is the effect file for the skydome
matrix WorldViewProj : WORLDVIEWPROJECTION;
float3 LightDir : LIGHTDIRECTION;
vector vSunColorIntensity : SUNCOLORINTENSITY;
float3 vBetaRayleigh : BETARAYLEIGH;
float3 vBetaDashRayleigh : BETADASHRAYLEIGH;
float3 vBetaMie : BETAMIE;
float3 vBetaDashMie : BETADASHMIE;
float3 vBetaRayleighMie : BETARAYLEIGHMIE;
float3 vOneOverBetaRM : ONEOVERBETARAYLEIGHMIE;
float4 vHG : HENYEYGG;
float3 vEyePos : EYEPOSITION;
float4 vDensityAlt : DENSDIST;
float3 vToneMap : TONEMAP;
float4 vConstants = {1.0f, -1.4426950f, 0.01f, 1000.0f}; // constants

struct VS_OUTPUT
{
    float4 Pos : POSITION;
    float4 Diff : COLOR0;
};

VS_OUTPUT VS(float3 vPos : POSITION, float3 Norm : NORMAL, float2 Tex : TEXCOORD)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;
    // transform
    Out.Pos = mul(float4(vPos, 1), WorldViewProj);
    // determine angle between sun and view direction
    float4 viewAngle;
    viewAngle.x = dot(LightDir, Norm);
    viewAngle.y = (viewAngle.x * viewAngle.x) / 2 + 2;
    viewAngle.z = lerp(Tex.y, vDensityAlt.z, Tex.x);
    viewAngle.w = Tex.y;
    viewAngle.w = lerp(viewAngle.w, vDensityAlt.w, Tex.x);

    // calculate extinction terms for in-scattered extinction
    float3 extinction;
	extinction = vBetaRayleigh * viewAngle.z + vBetaMie * viewAngle.w;
	extinction = exp(-extinction);

	// calculate mie scattering term
	// Phase 2 (theta) = (1 - g^2) / (1 + g^2 - 2g * cos(theta))^ (3/2)
	// vHG = [1 - g^2, 1 + g^2, -2g, insc]
	float4 phaseThetaMie;
	phaseThetaMie.x = vHG.z * viewAngle.x + vHG.y;
	phaseThetaMie.y = rsqrt(phaseThetaMie.x);
	phaseThetaMie.z = pow(phaseThetaMie.y, 3);
	phaseThetaMie.w = phaseThetaMie.z * vHG.x;

	// Inscattering (I) = (Beta R * Phase R(theta) + Beta M * Phase M(theta)) *
	// [1 - exp(-Beta R * s) * exp(-Beta M * s)] / (Beta R + Beta M)
	float3 rayleigh;
	rayleigh = vBetaDashRayleigh * viewAngle.y;
	float3 mie;
	mie = vBetaDashMie * phaseThetaMie.w;
	float3 temp;
	temp = vConstants.x - extinction;
	float3 inscatter; // I (inscattering)
	inscatter = (mie + rayleigh) * vOneOverBetaRM;
	inscatter *= temp;
	inscatter *= vSunColorIntensity.xyz;
	inscatter *= vSunColorIntensity.w;

	// color
	Out.Diff.xyz = inscatter;
	Out.Diff.z += 0.15f;
	Out.Diff.w = 1.0;

    return Out;
}
