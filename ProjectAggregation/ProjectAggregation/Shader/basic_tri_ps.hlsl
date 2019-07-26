
struct PS_IN {
	float4 pos  : SV_POSITION;
	float4 posw : POSITION0;	//ワールド座標系の座標
	float4 norw : NORMAL0;		//ワールド座標系の法線
	float2 tex  : TEXCOORD0;
};

struct Light {
	float4 pos;			//座標(x,y,z)
	float4 diffuse;		//拡散(r,g,b)
	float4 specular;	//反射(r,g,b)
	float4 attenuate;	//減衰(a,b,c)
};

struct Material {
	float4 ambient;		//環境(r,g,b)
	float4 diffuse;		//拡散(r,g,b)
	float4 specular;	//反射(r,g,b,光沢度係数)
};
 
cbuffer ConstantBuffer {
	float4   eyePos;	//視点座標
	float4   ambient;	//環境光(r,g,b)
	Light    pntLight;	//点光源
	Material material;	//物体の質感
};
 
Texture2D    myTexture : register(t0); //テクスチャー
SamplerState mySampler : register(s0); //サンプラー

float4 ps_main( PS_IN input ) : SV_Target
{
	float3 n;
	float3 v;
	float3 l;
	float3 r;
	float  d;
	float  a;
	float4 iT;
	float3 iA;
	float3 iD;
	float3 iS;

	n = normalize(input.norw.xyz);
	v = normalize(eyePos.xyz - input.posw.xyz);
	l = pntLight.pos.xyz - input.posw.xyz;
	d = length(l);		//光源距離
	l = normalize(l);	//正規化光源ベクトル
	r = 2.0 * n * dot(n, l) - l;	//正規化反射ベクトル
	a = saturate(1.0f / (pntLight.attenuate.x + pntLight.attenuate.y * d + pntLight.attenuate.z * d * d)); //減衰
	iT = myTexture.Sample(mySampler, input.tex);
	iA = iT * material.ambient.xyz  * ambient.xyz;
	iD = saturate(dot(l, n)) * iT * material.diffuse.xyz  * pntLight.diffuse.xyz  * a;
	iS = pow(saturate(dot(r, v)), material.specular.w) * material.specular.xyz * pntLight.specular.xyz * a;
	return float4(saturate(iA + iD + iS), iT.w);
}
