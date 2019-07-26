
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
	float4 iT;

	iT = myTexture.Sample(mySampler, input.tex);
	return float4(saturate(iT));
}
