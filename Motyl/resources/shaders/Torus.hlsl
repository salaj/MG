/*Texture2D colorMap : register(t0);
SamplerState colorSampler : register(s0);*/

cbuffer cbWorld : register(b0) //Vertex Shader constant buffer slot 0
{
	matrix worldMatrix;
};

cbuffer cbView : register(b1) //Vertex Shader constant buffer slot 1
{
	matrix viewMatrix;
};

cbuffer cbProj : register(b2) //Vertex Shader constant buffer slot 2
{
	matrix projMatrix;
};

cbuffer color : register(b0) //Vertex Shader constant buffer slot 2
{
	float4 color;
};

//cbuffer cbLightPos : register(b3) //Vertex Shader constant buffer slot 3
//{
//	float4 lightPos[3];
//}
//
//cbuffer cbLightColors : register(b0) //Pixel Shader constant buffer slot 0
//{
//	float4 ambientColor;
//	float4 surface; //[ka, kd, ks, m]
//	float4 lightColors[3];
//}

//cbuffer cbSurfaceColor : register(b1) //Pixel Shader constant buffer slot 0
//{
//	float4 surfaceColor;
//}

struct VSInput
{
	float3 pos : POSITION;
	//float3 color : COLOR0;
	float3 norm : NORMAL0;
};

struct PSInput
{
	float4 pos : SV_POSITION;
	//float3 color : COLOR;
	float3 norm : NORMAL;
	//float3 viewVec : TEXCOORD0;
	//float3 lightVec0 : TEXCOORD1;
	//float3 lightVec1 : TEXCOORD2;
	//float3 lightVec2 : TEXCOORD3;
};

PSInput VS_Main(VSInput i)
{
	PSInput o = (PSInput)0;
	float4 viewPos = float4(i.pos, 1.0f);

	/*matrix worldView = mul(viewMatrix, worldMatrix);
	viewPos = mul(worldView, viewPos);*/
	viewPos = mul(worldMatrix, viewPos);
	o.pos = mul(projMatrix, viewPos);

	//i.pos.w = 1.0f;
	//float4 viewPos = float4(i.pos, 1.0f);

	//o.pos = mul(viewPos, worldMatrix);
	//o.pos = mul(o.pos, viewMatrix);
	//o.pos = mul(o.pos, projMatrix);

	//o.color = i.color;

	//o.norm = mul(worldView, float4(i.norm, 0.0f)).xyz;
	//o.norm = normalize(o.norm);
	o.norm = i.norm;
	//o.color = i.color;

	//o.viewVec = normalize(-viewPos.xyz);

	//o.lightVec0 = normalize((mul(viewMatrix, lightPos[0]) - viewPos).xyz);
	//o.lightVec1 = normalize((mul(viewMatrix, lightPos[1]) - viewPos).xyz);
	//o.lightVec2 = normalize((mul(viewMatrix, lightPos[2]) - viewPos).xyz);
	return o;
}

float4 PS_Main(PSInput i) : SV_TARGET
{
	//float3 viewVec = normalize(i.viewVec);
	//float3 normal = normalize(i.norm);
	//float3 color = ambientColor.xyz * surface.x;


	//float3 lightVec;
	//float3 halfVec;
	////Light0
	//if (lightColors[0].w)
	//{
	//	lightVec = normalize(i.lightVec0);
	//	halfVec = normalize(viewVec + lightVec);
	//	color += lightColors[0].xyz * surfaceColor.xyz * surface.y * clamp(dot(normal, lightVec), 0.0f, 1.0f); //diffuse Color
	//	float nh = dot(normal, halfVec);
	//	nh = clamp(nh, 0.0f, 1.0f);
	//	nh = pow(nh, surface.w);
	//	color += lightColors[0].xyz * surface.z * nh; //specular Color
	//}
	////Light1
	//if (lightColors[1].w)
	//{
	//	lightVec = normalize(i.lightVec1);
	//	halfVec = normalize(viewVec + lightVec);
	//	color += lightColors[1].xyz * surfaceColor.xyz * surface.y * clamp(dot(normal, lightVec), 0.0f, 1.0f); //diffuse Color
	//	color += lightColors[1].xyz * surface.z * pow(clamp(dot(normal, halfVec), 0.0f, 1.0f), surface.w); //specular Color
	//}
	////Light2
	//if (lightColors[2].w)
	//{
	//	lightVec = normalize(i.lightVec2);
	//	halfVec = normalize(viewVec + lightVec);
	//	color += lightColors[2].xyz * surfaceColor.xyz * surface.y * clamp(dot(normal, lightVec), 0.0f, 1.0f); //diffuse Color
	//	color += lightColors[2].xyz * surface.z * pow(clamp(dot(normal, halfVec), 0.0f, 1.0f), surface.w); //specular Color
	//}
	return float4(color.xyz, 1.0f);
}